#include "Rasterizer.h"
#include "Fragmentprocessor.h"
#include "FrameBuffer.h"
#include "Vertex.h"

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
