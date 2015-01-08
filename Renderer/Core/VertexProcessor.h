#pragma once

#include "utMath.h"

class Vertex;
class Renderer;

class VertexProcessor
{
public:
    virtual void updateTransforms(Renderer &renderer) = 0;
    virtual void line(Vec3f *v, Vec3f *c, Vertex *vert) = 0;
    virtual void triangle(Vec3f *v, Vec3f *n, Vec3f *c, Vec3f *t, Vertex *vert) = 0;

protected:
    Matrix4f _mvpMat;
    Matrix4f _mvMat;
    Matrix4f _vpMat;
};