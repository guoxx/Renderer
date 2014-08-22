#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstring>
#include "utMath.h"
#include "FrameBuffer.h"


class Renderer {
public:
    Renderer() {
        assert(false);
    }

    Renderer(int w, int h) {
        m_width = w;
        m_height = h;
        m_buffer = (uint32_t*)malloc(sizeof(uint32_t) * m_width * m_height);
        bzero(m_buffer, sizeof(uint32_t) * m_width * m_height);
        
        frameBuffer = new FrameBuffer(m_width, m_height);
    }

    ~Renderer() {
        free(m_buffer);
        delete frameBuffer;
    }

    Matrix4f& getModelViewMatrix() { return m_modelviewMat; }
    Matrix4f& getProjectionMatrix() { return m_projectionMat; }
    Matrix4f& getViewportMatrix() { return m_viewportMat; }

    void lookat(Vec3f& eye, Vec3f& target, Vec3f& up);

    void setupViewParams(float fov, float aspect, float zNear, float zFar);
    
    void viewport(int x, int y, int w, int h);
    
    void clearColorBuffer(const Vec3f &color);
    
    void renderTriangle(int verticesCnt,
                        Vec3f *vertices,
                        Vec3f *colors,
                        Vec3f *normals,
                        Vec3f *textures);
    
    void drawDot(int x, int y, const Vec4f& color);
    
    bool dumpTga(const char* filename);

private:
    int m_width;
    int m_height;
    uint32_t* m_buffer;
    
    Matrix4f m_modelviewMat;
    Matrix4f m_projectionMat;
    Matrix4f m_viewportMat;
    
    FrameBuffer *frameBuffer;
};

#endif
