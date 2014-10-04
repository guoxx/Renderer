#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstring>
#include "utMath.h"
#include "FrameBuffer.h"


class Texture;

class Renderer {
public:
    Renderer(int w, int h) {
        m_width = w;
        m_height = h;
        
        frameBuffer = new FrameBuffer(m_width, m_height);
    }

    ~Renderer() {
        delete frameBuffer;
    }

    int getWidth() { return m_width; }
    int getHeight() { return m_height; }
    Matrix4f& getModelViewMatrix() { return m_modelviewMat; }
    Matrix4f& getProjectionMatrix() { return m_projectionMat; }
    Matrix4f& getViewportMatrix() { return m_viewportMat; }

    void lookat(Vec3f& eye, Vec3f& target, Vec3f& up);

    void setupViewParams(float fov, float aspect, float zNear, float zFar);
    
    void viewport(int x, int y, int w, int h);
    
    void clearColorBuffer(const Vec3f &color);
    
    void renderLine(int lineCnt,
                    Vec3f *vertices,
                    Vec3f *colors);

    void renderTriangle(int verticesCnt,
                        Vec3f *vertices,
                        Vec3f *colors,
                        Vec3f *normals,
                        Vec3f *textures,
                        Texture *tex);

    void dumpRaw(uint8_t **data, int *sz);

private:
    int m_width;
    int m_height;
    
    Matrix4f m_modelviewMat;
    Matrix4f m_projectionMat;
    Matrix4f m_viewportMat;
    
    FrameBuffer *frameBuffer;
};

#endif
