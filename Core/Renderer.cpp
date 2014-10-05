#include "Renderer.h"
#include "Vertex.h"
#include "Fragment.h"
#include "VertexProcessor.h"
#include "FragmentProcessor.h"
#include "Rasterizer.h"
#include "FrameBuffer.h"
#include "Texture.h"

#include <stdio.h>
#include <unistd.h>
#include <math.h>

static uint32_t _color3ToUInt(const Vec3f& color){
    uint32_t c;
    uint8_t* p = (uint8_t*)&c;
    *p = uint8_t(std::fmin(color.x, 1.0f) * 255);
    *(p + 1) = uint8_t(std::fmin(color.y, 1.0f) * 255);
    *(p + 2) = uint8_t(std::fmin(color.z, 1.0f) * 255);
    *(p + 3) = 255;
    return c;
}

#pragma mark ----

void Renderer::lookat(Vec3f& eye, Vec3f& target, Vec3f& up){
    Vec3f u, v, w;
    w = eye - target;
    w.normalize();
    u = up.cross(w);
    u.normalize();
    v = w.cross(u);
    v.normalize();

    Matrix4f rotate;
    _modelviewMat.identity();
    rotate.c[0][0] = u.x;
    rotate.c[1][0] = u.y;
    rotate.c[2][0] = u.z;
    rotate.c[0][1] = v.x;
    rotate.c[1][1] = v.y;
    rotate.c[2][1] = v.z;
    rotate.c[0][2] = w.x;
    rotate.c[1][2] = w.y;
    rotate.c[2][2] = w.z;

    Matrix4f translate;
    translate.c[3][0] = -eye.x;
    translate.c[3][1] = -eye.y;
    translate.c[3][2] = -eye.z;

    _modelviewMat = rotate * translate;
}

void Renderer::setupViewParams(float fov, float aspect, float zNear, float zFar){
    float t = zNear * (float)tanf(fov / 180.0f);
    float b = -t;
    float r = t / aspect;
    float l = -r;
    float n = zNear;
    float f = zFar;
    _projectionMat.identity();
    _projectionMat.c[0][0] = (2 * n) / (r - l);
    _projectionMat.c[2][0] = (r + l) / (r - l);
    _projectionMat.c[1][1] = (2 * n) / (t - b);
    _projectionMat.c[3][1] = (t + b) / (t - b);
    _projectionMat.c[2][2] = (n + f) / (n - f);
    _projectionMat.c[3][2] = (2 * f * n) / (n - f);
    _projectionMat.c[2][3] = -1;
    _projectionMat.c[3][3] = 0;
}

void Renderer::viewport(int x, int y, int w, int h){
    int cx = x + w / 2;
    int cy = y + h / 2;
    _viewportMat.identity();
    _viewportMat.c[0][0] = w/2;
    _viewportMat.c[3][0] = cx;
    _viewportMat.c[1][1] = h/2;
    _viewportMat.c[3][1] = cy;
}

void Renderer::clearColorBuffer(const Vec3f &color){
    for (int x = 0; x < _width; x = x + 1)
    {
        for (int y = 0; y < _height; y = y + 1)
        {
            _frameBuffer->setColorBuffer(x, y, color);
        }
    }
}

void Renderer::renderLine(int verticesCnt,
                          Vec3f *vertices,
                          Vec3f *colors)
{
    Vertex *vertCache = new Vertex[verticesCnt];

    VertexProcessor vertProcessor;
    FragmentProcessor fragProcessor;
    Rasterizer rasterizer;

    vertProcessor.updateTransforms(*this);
    int lineCnt = verticesCnt / 2;
    for (int i = 0; i < lineCnt; i = i + 1)
    {
        vertProcessor.line(vertices + i * 2,
                           colors + i * 2,
                           vertCache + i * 2);
    }
    for (int i = 0; i < lineCnt; i = i + 1) {
        rasterizer.line(vertCache + i * 2, fragProcessor, *_frameBuffer);
    }

    delete [] vertCache;
}

void Renderer::renderTriangle(int verticesCnt,
                              Vec3f *vertices,
                              Vec3f *colors,
                              Vec3f *normals,
                              Vec3f *textures,
                              std::shared_ptr<Texture> tex) {
    // TODO: face culling

    Vertex *vertCache = new Vertex[verticesCnt];
    VertexProcessor vertProcessor;
    FragmentProcessor fragProcessor;
    Rasterizer rasterizer;

    vertProcessor.updateTransforms(*this);
    int triangleCnt = verticesCnt / 3;
    for (int i = 0; i < triangleCnt; i = i + 1) {
        vertProcessor.triangle(vertices + i * 3,
                               normals + i * 3,
                               colors + i * 3,
                               textures + i * 3,
                               vertCache + i * 3);
    }
    rasterizer.setupTexture(tex);
    for (int i = 0; i < triangleCnt; i = i + 1) {
        rasterizer.triangle(vertCache + i * 3, fragProcessor, *_frameBuffer);
    }

    delete [] vertCache;
}

void Renderer::dumpRaw(uint8_t **data, int *sz) {
    assert(data != NULL);

    *data = (uint8_t*)malloc(_width * _height * 3);
    for (int x = 0; x < _width; x = x + 1) {
        for (int y = 0; y < _height; y = y + 1) {
            uint32_t clr = _color3ToUInt(_frameBuffer->getColorBuffer(x, y));
            uint8_t* p = (uint8_t*)&clr;
            int idx = y * _width + x;
            uint8_t* tmp = (*data) + idx * 3;
            // write BGR
            *(tmp + 0) = *(p + 2);
            *(tmp + 1) = *(p + 1);
            *(tmp + 2) = *(p + 0);
        }
    }
    if (sz != NULL) {
        *sz = _width * _height * 3;
    }
}
