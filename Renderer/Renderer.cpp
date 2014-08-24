#include "Renderer.h"
#include "Vertex.h"
#include "Fragment.h"
#include "VertexProcessor.h"
#include "FragmentProcessor.h"
#include "Rasterizer.h"
#include "FrameBuffer.h"

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

static uint32_t _color4ToUInt(const Vec4f& color){
    uint32_t c;
    uint8_t* p = (uint8_t*)&c;
    *p = uint8_t(std::fmin(color.x, 1.0f) * 255);
    *(p + 1) = uint8_t(std::fmin(color.y, 1.0f) * 255);
    *(p + 2) = uint8_t(std::fmin(color.z, 1.0f) * 255);
    *(p + 3) = uint8_t(std::fmin(color.w, 1.0f) * 255);
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
    m_modelviewMat.identity();
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

    m_modelviewMat = rotate * translate;
}

void Renderer::setupViewParams(float fov, float aspect, float zNear, float zFar){
    float t = zNear * (float)tanf(fov / 180.0f);
    float b = -t;
    float r = t / aspect;
    float l = -r;
    float n = zNear;
    float f = zFar;
    l = -0.6;
    r = 0.6;
    b = - 0.6;
    t = 0.6;
    m_projectionMat.identity();
    m_projectionMat.c[0][0] = (2 * n) / (r - l);
    m_projectionMat.c[2][0] = (r + l) / (r - l);
    m_projectionMat.c[1][1] = (2 * n) / (t - b);
    m_projectionMat.c[3][1] = (t + b) / (t - b);
    m_projectionMat.c[2][2] = (n + f) / (n - f);
    m_projectionMat.c[3][2] = (2 * f * n) / (n - f);
    m_projectionMat.c[2][3] = -1;
    m_projectionMat.c[3][3] = 0;
}

void Renderer::viewport(int x, int y, int w, int h){
    int cx = x + w / 2;
    int cy = y + h / 2;
    m_viewportMat.identity();
    m_viewportMat.c[0][0] = w/2;
    m_viewportMat.c[3][0] = cx;
    m_viewportMat.c[1][1] = h/2;
    m_viewportMat.c[3][1] = cy;
    // TODO: xxxx, why?
    m_viewportMat.c[2][2] = 0.5;
    m_viewportMat.c[3][2] = 0.5;
}

void Renderer::clearColorBuffer(const Vec3f &color){
    uint32_t c = _color3ToUInt(color);
    int i = 0;
    while (i < m_width * m_height) {
        m_buffer[i] = c;
        i = i + 1;
    }
}

void Renderer::renderTriangle(int verticesCnt,
                              Vec3f *vertices,
                              Vec3f *colors,
                              Vec3f *normals,
                              Vec3f *textures)
{
    // TODO: face culling

    Vertex *vertCache = new Vertex[verticesCnt];
    VertexProcessor *vertProcessor = new VertexProcessor;
    FragmentProcessor *fragProcessor = new FragmentProcessor;
    Rasterizer *rasterizer = new Rasterizer(m_width, m_height);

    vertProcessor->updateTransforms(*this);
    int triangleCnt = verticesCnt / 3;
    for (int i = 0; i < triangleCnt; i = i + 1) {
        vertProcessor->triangle(vertices + i * 3,
                                normals + i * 3,
                                colors + i * 3,
                                textures + i * 3,
                                vertCache + i * 3);
    }
    for (int i = 0; i < triangleCnt; i = i + 1) {
        rasterizer->rasterize(vertCache + i * 3, fragProcessor, frameBuffer);
    }
    for (int x = 0; x < m_width; x = x + 1) {
        for (int y = 0; y < m_height; y = y + 1) {
            drawDot(x, y, Vec4f(frameBuffer->getColorBuffer(x, y)));
        }
    }

    delete fragProcessor;
    delete vertProcessor;
    delete rasterizer;
    delete [] vertCache;
}

void Renderer::drawDot(int x, int y, const Vec4f& color){
    assert(x < m_width && y < m_height);
    
    uint32_t c = _color4ToUInt(color);
    m_buffer[y * m_width + x] = c;
}

bool Renderer::dumpTga(const char* filename) {
#pragma pack(1)
    typedef struct {
        char  idlength;
        char  colourmaptype;
        char  datatypecode;
        short int colourmaporigin;
        short int colourmaplength;
        char  colourmapdepth;
        short int x_origin;
        short int y_origin;
        short width;
        short height;
        char  bitsperpixel;
        char  imagedescriptor;
    } HEADER;
#pragma pack()

    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        return false;
    }

    assert(sizeof(HEADER) == 18);
    HEADER hdr;
    hdr.idlength = 0;
    hdr.colourmaptype = 0;
    hdr.datatypecode = 2;
    hdr.colourmaporigin = 0;
    hdr.colourmaplength = 0;
    hdr.colourmapdepth = 0;
    hdr.x_origin = 0;
    hdr.y_origin = 0;
    hdr.width = m_width;
    hdr.height = m_height;
    hdr.bitsperpixel = 24;
    hdr.imagedescriptor = 0;
    fwrite(&hdr, sizeof(uint8_t), 18, f);
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            int idx = r * m_width + c;
            uint32_t clr = m_buffer[idx];
            uint8_t* p = (uint8_t*)&clr;
            // write BGR
            fwrite(p + 2, sizeof(uint8_t), 1, f);
            fwrite(p + 1, sizeof(uint8_t), 1, f);
            fwrite(p + 0, sizeof(uint8_t), 1, f);
        }
    }
    fclose(f);
    return true;
}
;
