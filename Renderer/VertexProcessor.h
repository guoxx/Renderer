#ifndef __VERTEXPROCESSOR_H__
#define __VERTEXPROCESSOR_H__

#include "utMath.h"

class Vertex;
class Renderer;

class VertexProcessor {
public:
    void updateTransforms(Renderer &renderer);
    void triangle(Vec3f *v, Vec3f *n, Vec3f *c, Vec3f *t, Vertex *vert);
    
private:
    Matrix4f m_mvpMat;
    Matrix4f m_mvMat;
    Matrix4f m_vpMat;
};

#endif
