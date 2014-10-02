#include "Rasterizer.h"
#include "Fragmentprocessor.h"
#include "FrameBuffer.h"
#include "Vertex.h"

void Rasterizer::lineSegment(Vertex *vertices, FragmentProcessor &fp, FrameBuffer &fb)
{
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

    int x0 = p[i0].x;
    int y0 = p[i0].y;
    int z0 = p[i0].z;
    int x1 = p[i1].x;
    int y1 = p[i1].y;
    int z1 = p[i1].z;
    Vec3f deltaColor = (cl[i1] - cl[i0]) / (p[i0] - p[i1]).length();

    #define f(x,y) (y1 - y0)*(x) + (x0 - x1)*(y) + (x1*y0 - x0*y1)

    int a = int(y1 - y0);
    int b = int(x0 - x1);
    int c = int(x1 * y0 - x0 * y1);
    Vec3f color = cl[i0];
    assert(b <= 0);
    if (a <= b)
    {
        // (-inf, -1]
        int x = x0;
        float d = f(x0 + 0.5f, y0 - 1);
        for (int y = y0; y >= y1; y = y - 1)
        {
            color = color + deltaColor;
            fb.setColorBuffer(x, y, color);

            if (d >= 0)
            {
                x = x + 1;
                d = d + a - b;
            }
            else
            {
                d = d - b;
            }
        }
    }
    else if (b < a && a <= 0)
    {
        // (-1, 0]
        int y = y0;
        float d = f(x0 + 1, y0 - 0.5f);
        for (int x = x0; x <= x1; x = x + 1)
        {
            color = color + deltaColor;
            fb.setColorBuffer(x, y, color);
            
            if (d <= 0)
            {
                y = y - 1;
                d = d + a - b;
            }
            else
            {
                d = d + a;
            }
        }
    }
    else if (0 < a && a <= -b)
    {
        // (0, 1]
        int y = y0;
        float d = f(x0 + 1, y0 + 0.5f);
        for (int x = x0; x <= x1; x = x + 1)
        {
            color = color + deltaColor;
            fb.setColorBuffer(x, y, color);
            
            if (d >= 0)
            {
                y = y + 1;
                d = d + a + b;
            }
            else
            {
                d = d + a;
            }
        }
    }
    else if (-b < a)
    {
        // (1, +inf)
        int x = x0;
        float d = f(x0 + 0.5f, y0 + 1);
        for (int y = y0; y <= y1; y = y + 1)
        {
            color = color + deltaColor;
            fb.setColorBuffer(x, y, color);
            
            if (d <= 0)
            {
                x = x + 1;
                d = d + a + b;
            }
            else
            {
                d = d + b;
            }
        }
    }
    else
    {
        assert(0);
    }
    
    #undef f
}

void Rasterizer::triangle(Vertex *vertices, FragmentProcessor *fp, FrameBuffer *fbuffer){
    Vertex &a = *vertices;
    Vertex &b = *(vertices + 1);
    Vertex &c = *(vertices + 2);
    #define CONVERT(val) \
    Vec3f(val.vert.x/val.vert.w, \
          val.vert.y/val.vert.w, \
          val.vert.z/val.vert.w)
    Vec3f pa = CONVERT(a);
    Vec3f pb = CONVERT(b);
    Vec3f pc = CONVERT(c);
    #undef CONVERT
    Vec3f colorA = a.color;
    Vec3f colorB = b.color;
    Vec3f colorC = c.color;

    int minx = floorf(minf(minf(pa.x, pb.x), pc.x));
    int maxx = ceilf(maxf(maxf(pa.x, pb.x), pc.x));
    int miny = floorf(minf(minf(pa.y, pb.y), pc.y));
    int maxy = ceilf(maxf(maxf(pa.y, pb.y), pc.y));

    #define f(p1, p0, xvar, yvar) ((p1.y - p0.y)*(xvar) + (p0.x - p1.x)*(yvar) + (p1.x*p0.y - p0.x*p1.y))

    // TODO: frustum culling
    float fa = f(pc, pb, pa.x, pa.y);
    float fb = f(pc, pa, pb.x, pb.y);
    float fg = f(pb, pa, pc.x, pc.y);
    for (int x = minx; x <= maxx; x = x + 1) {
        for (int y = miny; y <= maxy; y = y + 1) {
            float alpha = f(pc, pb, x, y) / fa;
            float beta = f(pc, pa, x, y) / fb;
            float gamma = f(pb, pa, x, y) / fg;
            if (alpha > 0 && beta > 0 && gamma > 0) {
                float z = pa.z + beta * (pb.z - pa.z) + gamma * (pc.z - pa.z);
                (void)z;
                // TODO: handle depth buffer

                if (_tex != NULL) {
                    float h0 = a.vert.w;
                    float h1 = b.vert.w;
                    float h2 = c.vert.w;
                    float denominator = h1*h2 + h2*beta*(h0-h1) + h1*gamma*(h0-h2);
                    float beta_w = h0 * h2 * beta / denominator;
                    float gamma_w = h0 * h1 * gamma / denominator;
                    float alpha_w = 1.0f - beta_w - gamma_w;

                    float u = a.texCoords.u * alpha_w + b.texCoords.u * beta_w + c.texCoords.u * gamma_w;
                    float v = a.texCoords.v * alpha_w + b.texCoords.v * beta_w + c.texCoords.v * gamma_w;
                    Vec3f color = _tex->getColor(u, v);
                    fbuffer->setColorBuffer(x, y, color);
                }
                else {
                    Vec3f color = colorA + (colorB - colorA) * beta + (colorC - colorA) * gamma;
                    fbuffer->setColorBuffer(x, y, color);
                }
            }
        }
    }
    
    #undef f
}
