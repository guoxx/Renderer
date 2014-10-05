#include "VertexProcessor.h"
#include "Vertex.h"
#include "Renderer.h"

#include <cassert>

void VertexProcessor::updateTransforms(Renderer &renderer) {
    _mvMat = renderer.getModelViewMatrix();
    _mvpMat = renderer.getViewportMatrix() * renderer.getProjectionMatrix() * _mvMat;
    _vpMat = renderer.getViewportMatrix();
}

void VertexProcessor::line(Vec3f *v, Vec3f *c, Vertex *vert) {
    for (int i = 0; i < 2; i = i + 1) {
        (vert + i)->vert = _mvpMat * (Vec4f(*(v + i)));
        (vert + i)->color = *(c + i);
    }
}

void VertexProcessor::triangle(Vec3f *v, Vec3f *n, Vec3f *c, Vec3f *t, Vertex *vert) {
    for (int i = 0; i < 3; i = i + 1) {
        (vert + i)->vert = _mvpMat * (Vec4f(*(v + i)));
        // caculate normal and color
        (vert + i)->normal = *(n + i);
        (vert + i)->color = *(c + i);
        (vert + i)->vEye = _mvMat * (*(v + i));
        if (t != NULL) {
            (vert + i)->texCoord.x = (t + i)->x;
            (vert + i)->texCoord.y = (t + i)->y;
        }
    }
}