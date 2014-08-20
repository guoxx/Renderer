#include "VertexProcessor.h"
#include "Vertex.h"
#include "Renderer.h"

#include <cassert>

void VertexProcessor::updateTransforms(Renderer &renderer){
    m_mvMat = renderer.getModelViewMatrix();
    m_mvpMat = renderer.getProjectionMatrix() * m_mvMat;
    m_vpMat = renderer.getViewportMatrix();
}

void VertexProcessor::triangle(Vec3f *v, Vec3f *n, Vec3f *c, Vec3f *t, Vertex *vert){
    for (int i = 0; i < 3; i = i + 1) {
        vert->vert = m_mvpMat * (Vec4f(*v));
        // caculate normal and color
        vert->normal = *n;
        vert->color = *c;
        vert->vEye = m_mvMat * (*v);
    }
}