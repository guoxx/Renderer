#ifndef __RENDERER__VERTEX__
#define __RENDERER__VERTEX__

#include "utMath.h"

typedef struct Vertex {
    Vec4f vert;
    Vec3f normal;
    Vec3f color;
    Vec3f vEye;
    struct {
        float u, v;
    } texCoords;
} Vertex;

#endif /* defined(__RENDERER__VERTEX__) */
