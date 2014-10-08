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

static int _argMin(int cnt, double v, ...) {
    int idx = 0, cursor = 0;
    float min = v;
    cnt = cnt - 1;

    va_list args;
    va_start(args, v);
    while (cursor < cnt) {
        float x = va_arg(args, double);
        if (x < min) {
            idx = cursor;
        }
        cursor = cursor + 1;
    }
    va_end(args);
    return idx;
}

static Vec3f _nonParallelVector(Vec3f v) {
    v.normalize();

    int i = _argMin(3, v.x, v.y, v.z);
    Vec3f u(v);
    if (i == 0)
        u.x = 1;
    else if (i == 1)
        u.y = 1;
    else if (i == 2)
        u.z = 1;
    return u;
}

#pragma mark ----

void Renderer::orbit(Vec2f delta) {
//    printf("delta = %f, %f\n", delta.x, delta.y);

    Vec3f u, v, w;
    w = _eye - _target;
    w.normalize();
    Vec3f up = _nonParallelVector(w);
    u = up.cross(w);
    u.normalize();
    v = w.cross(u);

    Matrix4f basis;
    basis.identity();
    basis.c[0][0] = u.x;
    basis.c[0][1] = u.y;
    basis.c[0][2] = u.z;
    basis.c[1][0] = v.x;
    basis.c[1][1] = v.y;
    basis.c[1][2] = v.z;
    basis.c[2][0] = w.x;
    basis.c[2][1] = w.y;
    basis.c[2][2] = w.z;

    Matrix4f basisInv = basis.inverted();

    Vec3f newEye = basisInv * _eye;
    float r = newEye.length();
    float phi = atan2f(newEye.y, newEye.x);
    float theta = asinf(newEye.z / r);

    // increment phi and theta by mouse motion
    printf("delta phi = %f\n", 3.14f / 2 * delta.x);
    printf("delta theta = %f\n", 3.14f / 2 * delta.y);

    phi = phi - 3.14f / 2 * delta.x;
    theta = theta - 3.14f / 2 * delta.y;
    float thetaLimit = (float) (89 * M_PI / 180);
    if (theta > thetaLimit)
        theta = thetaLimit;
    if (theta < -thetaLimit)
        theta = -thetaLimit;

    newEye.x = r * cosf(theta) * cosf(phi);
    newEye.y = r * cosf(theta) * sinf(phi);
    newEye.z = r * sinf(theta);
    newEye = basis * newEye;
    printf("old eye = %f, %f, %f\n", _eye.x, _eye.y, _eye.z);
    printf("new eye = %f, %f, %f\n", newEye.x, newEye.y, newEye.z);
    lookat(newEye, _target, _up);
}

void Renderer::dolly(float delta) {

}

void Renderer::lookat(Vec3f& eye, Vec3f& target, Vec3f& up){
    _eye = eye;
    _target = target;
    _up = up;

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

void Renderer::setupPipeline(std::shared_ptr<VertexProcessor> vp, std::shared_ptr<FragmentProcessor> fp) {
    _vp = vp;
    _fp = fp;
}

void Renderer::clearDepthBuffer(float val) {
    for (int x = 0; x < _width; x = x + 1) {
        for (int y = 0; y < _height; y = y + 1) {
            _frameBuffer->setDepthBuffer(x, y, val);
        }
    }
}

void Renderer::clearColorBuffer(const Vec3f &color) {
    for (int x = 0; x < _width; x = x + 1) {
        for (int y = 0; y < _height; y = y + 1) {
            _frameBuffer->setColorBuffer(x, y, color);
        }
    }
}

void Renderer::renderLine(int verticesCnt,
                          Vec3f *vertices,
                          Vec3f *colors) {
    Vertex *vertCache = new Vertex[verticesCnt];

    Rasterizer rasterizer;

    _vp->updateTransforms(*this);
    int lineCnt = verticesCnt / 2;
    for (int i = 0; i < lineCnt; i = i + 1) {
        _vp->line(vertices + i * 2, colors + i * 2, vertCache + i * 2);
    }
    for (int i = 0; i < lineCnt; i = i + 1) {
        rasterizer.line(vertCache + i * 2, *_fp, *_frameBuffer);
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
    Rasterizer rasterizer;

    _vp->updateTransforms(*this);
    int triangleCnt = verticesCnt / 3;
    for (int i = 0; i < triangleCnt; i = i + 1) {
        _vp->triangle(vertices + i * 3,
                      normals + i * 3,
                      colors + i * 3,
                      textures + i * 3,
                      vertCache + i * 3);
    }
    rasterizer.setupTexture(tex);
    for (int i = 0; i < triangleCnt; i = i + 1) {
        rasterizer.triangle(vertCache + i * 3, *_fp, *_frameBuffer);
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
