#include "Rasterizer.h"
#include "Fragmentprocessor.h"
#include "FrameBuffer.h"
#include "Vertex.h"

void Rasterizer::line(Vertex *vertices, FragmentProcessor &fp, FrameBuffer &framebuffer) {
    // TODO: perspective correct z and color interpolation
    Vec3f p[2];
    Vec3f cl[2];

    Vertex &vertA = *vertices;
    Vertex &vertB = *(vertices + 1);
    #define CONVERT(val) \
    Vec3f(val.vert.x/val.vert.w, \
          val.vert.y/val.vert.w, \
          val.vert.z/val.vert.w)
    p[0] = CONVERT(vertA);
    p[1] = CONVERT(vertB);
    cl[0] = vertA.color;
    cl[1] = vertB.color;
    #undef CONVERT

    // f(x,y) = (y1 - y0)*x + (x0 - x1)*y + (x1*y0 - x0*y1) = a*x + b*y + c;
    int i0 = (p[0].x <= p[1].x) ? 0 : 1;
    int i1 = (i0 + 1) % 2;

    int x0 = static_cast<int>(p[i0].x);
    int y0 = static_cast<int>(p[i0].y);
    float z0 = p[i0].z;
    int x1 = static_cast<int>(p[i1].x);
    int y1 = static_cast<int>(p[i1].y);
    float z1 = p[i1].z;
    float len = (p[i0] - p[i1]).length();
    Vec3f deltaColor = (cl[i1] - cl[i0]) / len;

    #define f(x,y) (y1 - y0)*(x) + (x0 - x1)*(y) + (x1*y0 - x0*y1)

    int a = int(y1 - y0);
    int b = int(x0 - x1);
    Vec3f color = cl[i0];
    assert(b <= 0);
    if (a <= b) {
        // (-inf, -1]
        int x = x0;
        float d = f(x0 + 0.5f, y0 - 1);
        for (int y = y0; y >= y1; y = y - 1) {
            color = color + deltaColor;
            framebuffer.setColorBuffer(x, y, color);

            if (d >= 0) {
                x = x + 1;
                d = d + a - b;
            }
            else {
                d = d - b;
            }
        }
    }
    else if (b < a && a <= 0) {
        // (-1, 0]
        int y = y0;
        float d = f(x0 + 1, y0 - 0.5f);
        for (int x = x0; x <= x1; x = x + 1) {
            color = color + deltaColor;
            framebuffer.setColorBuffer(x, y, color);
            
            if (d <= 0) {
                y = y - 1;
                d = d + a - b;
            }
            else {
                d = d + a;
            }
        }
    }
    else if (0 < a && a <= -b) {
        // (0, 1]
        int y = y0;
        float d = f(x0 + 1, y0 + 0.5f);
        for (int x = x0; x <= x1; x = x + 1) {
            color = color + deltaColor;
            framebuffer.setColorBuffer(x, y, color);
            
            if (d >= 0) {
                y = y + 1;
                d = d + a + b;
            }
            else {
                d = d + a;
            }
        }
    }
    else if (-b < a) {
        // (1, +inf)
        int x = x0;
        float d = f(x0 + 0.5f, y0 + 1);
        for (int y = y0; y <= y1; y = y + 1) {
            color = color + deltaColor;
            framebuffer.setColorBuffer(x, y, color);
            
            if (d <= 0) {
                x = x + 1;
                d = d + a + b;
            }
            else {
                d = d + b;
            }
        }
    }
    else {
        assert(0);
    }
    
    #undef f
}

void Rasterizer::triangle(Vertex *vertices, FragmentProcessor &fp, FrameBuffer &framebuffer) {
    Vertex &v0 = *vertices;
    Vertex &v1 = *(vertices + 1);
    Vertex &v2 = *(vertices + 2);

    auto invw = [](Vertex v) {
        return Vec3f(v.vert.x/v.vert.w, v.vert.y/v.vert.w, v.vert.z/v.vert.w);
    };

    Vec3f p0 = invw(v0);
    Vec3f p1 = invw(v1);
    Vec3f p2 = invw(v2);

    auto min3f = [](float f0, float f1, float f2) { return minf(minf(f0, f1), f2); };
    auto max3f = [](float f0, float f1, float f2) { return maxf(maxf(f0, f1), f2); };
    
	int minx{static_cast<int>(floorf(min3f(p0.x, p1.x, p2.x)))};
	int maxx{static_cast<int>(ceilf(max3f(p0.x, p1.x, p2.x)))};
	int miny{static_cast<int>(floorf(min3f(p0.y, p1.y, p2.y)))};
	int maxy{static_cast<int>(ceilf(max3f(p0.y, p1.y, p2.y)))};

    auto f = [](Vec3f& p1, Vec3f& p0, Vec3f& p) {
        return ((p1.y - p0.y) * p.x + (p0.x - p1.x) * p.y + (p1.x * p0.y - p0.x * p1.y));
    };

    // TODO: frustum culling
    float fa = f(p2, p1, p0);
    float fb = f(p2, p0, p1);
    float fg = f(p1, p0, p2);
    for (int x = minx; x <= maxx; x = x + 1) {
        for (int y = miny; y <= maxy; y = y + 1) {
            float alpha = f(p2, p1, Vec3f(x, y, 0)) / fa;
            float beta = f(p2, p0, Vec3f(x, y, 0)) / fb;
            float gamma = f(p1, p0, Vec3f(x, y, 0)) / fg;
            if (alpha > 0 && beta > 0 && gamma > 0) {
                float z = p0.z + beta * (p1.z - p0.z) + gamma * (p2.z - p0.z);
                float zval = framebuffer.getDepthBuffer(x, y);
                if (z > zval)
                    continue;

                if (_tex != NULL) {
                    float h0 = v0.vert.w;
                    float h1 = v1.vert.w;
                    float h2 = v2.vert.w;
                    float denominator = h1*h2 + h2*beta*(h0-h1) + h1*gamma*(h0-h2);
                    float beta_w = h0 * h2 * beta / denominator;
                    float gamma_w = h0 * h1 * gamma / denominator;
                    float alpha_w = 1.0f - beta_w - gamma_w;

                    float u = v0.texCoord.x * alpha_w + v1.texCoord.x * beta_w + v2.texCoord.x * gamma_w;
                    float v = v0.texCoord.y * alpha_w + v1.texCoord.y * beta_w + v2.texCoord.y * gamma_w;
                    Vec3f color = _tex->getColor(u, v);
                    framebuffer.setColorBuffer(x, y, color);
                    framebuffer.setDepthBuffer(x, y, z);
                }
                else {
                    Vec3f color = v0.color + (v1.color - v0.color) * beta + (v2.color - v0.color) * gamma;
                    framebuffer.setColorBuffer(x, y, color);
                    framebuffer.setDepthBuffer(x, y, z);
                }
            }
        }
    }
}
