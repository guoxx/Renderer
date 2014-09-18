#include "Rasterizer.h"
#include "Fragmentprocessor.h"
#include "FrameBuffer.h"
#include "Vertex.h"

void Rasterizer::lineSegment(Vertex *vertices, FragmentProcessor &fp, FrameBuffer &fb)
{
    Vec3f p[2];

    Vertex &vertA = *vertices;
    Vertex &vertB = *(vertices + 1);
    #define CONVERT(val) \
    Vec3f(val.vert.x/val.vert.w, \
          val.vert.y/val.vert.w, \
          val.vert.z/val.vert.w)
    p[0] = CONVERT(vertA);
    p[1] = CONVERT(vertB);
    #undef CONVERT
    Vec3f colorA = vertA.color;
    Vec3f colorB = vertB.color;

    // f(x,y) = (y1 - y0)*x + (x0 - x1)*y + (x1*y0 - x0*y1) = a*x + b*y + c;
    int i0 = (p[0].x <= p[1].x) ? 0 : 1;
    int i1 = (i0 + 1) % 2;
    
    int x0 = p[i0].x;
    int y0 = p[i0].y;
    int z0 = p[i0].z;
    int x1 = p[i1].x;
    int y1 = p[i1].y;
    int z1 = p[i1].z;
    #define f(x,y) (y1 - y0)*(x) + (x0 - x1)*(y) + (x1*y0 - x0*y1)
    
    int a = int(y1 - y0);
    int b = int(x0 - x1);
    int c = int(x1 * y0 - x0 * y1);
    assert(b <= 0);
    if (a <= b)
    {
        // (-inf, -1]
        int x = x0;
        float d = f(x0 + 0.5f, y0 - 1);
        for (int y = y0; y >= y1; y = y - 1)
        {
            Vec3f color(255, 0, 0);
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
            Vec3f color(255, 0, 0);
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
            Vec3f color(255, 0, 0);
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
            Vec3f color(255, 0, 0);
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
}

void Rasterizer::rasterize(Vertex *vertices, FragmentProcessor *fp, FrameBuffer *fb){
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

    // TODO: frustum culling
    for (float x = 0; x < width; ++x) {
        for (float y = 0; y < height; ++y) {
            float beta = ((pa.y - pc.y)*x + (pc.x - pa.x)*y + pa.x*pc.y - pc.x*pa.y) /
                         ((pa.y - pc.y)*pb.x + (pc.x - pa.x)*pb.y + pa.x*pc.y - pc.x*pa.y);
            float gamma = ((pa.y - pb.y)*x + (pb.x - pa.x)*y + pa.x*pb.y - pb.x*pa.y) /
                          ((pa.y - pb.y)*pc.x + (pb.x - pa.x)*pc.y + pa.x*pb.y - pb.x*pa.y);

            if ((0.0f <= beta && beta <= 1.0f) && (0.0f <= gamma && gamma <= 1.0f) && (beta + gamma) <= 1.0f) {
                float z = pa.z + beta * (pb.z - pa.z) + gamma * (pc.z - pa.z);
                (void)z;
                // TODO: handle depth buffer

                Vec3f color = colorA + (colorB - colorA) * beta + (colorC - colorA) * gamma;
                fb->setColorBuffer(x, y, color);
            }
        }
    }
}
