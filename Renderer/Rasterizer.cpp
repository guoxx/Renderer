#include "Rasterizer.h"
#include "Fragmentprocessor.h"
#include "FrameBuffer.h"
#include "Vertex.h"

void Rasterizer::rasterize(Vertex *vertices, FragmentProcessor *fp, FrameBuffer *fb){
    Vertex *a = vertices;
    Vertex *b = vertices + 1;
    Vertex *c = vertices + 2;
    #define CONVERT(val) \
    Vec3f(val->vert.x/val->vert.w, \
          val->vert.y/val->vert.w, \
          val->vert.z/val->vert.w)
    Vec3f pa = CONVERT(a);
    Vec3f pb = CONVERT(b);
    Vec3f pc = CONVERT(c);
    #undef CONVERT

    // TODO: frustum culling
    for (float x = 0; x < width; ++x) {
        for (float y = 0; y < height; ++y) {
            float det = (pb.x - pa.x)*(pc.y - pa.y) - (pc.x - pa.x)*(pb.y - pa.y);
            float beta = (pc.y - pa.y)*(x - pa.x) - (pb.y - pa.y)*(y - pa.y);
            beta = beta / det;
            float gamma = - (pc.x - pa.x)*(x - pa.x) + (pb.x - pa.x)*(y - pa.y);
            gamma = gamma / det;

            if ((0.0f <= beta && beta <= 1.0f) && (0.0f <= gamma && gamma <= 1.0f)) {
                float z = pa.z + beta * (pb.z - pa.z) + gamma * (pc.z - pa.z);

                // TODO: handle depth buffer
                
            }
        }
    }
}
