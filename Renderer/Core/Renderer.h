#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstring>
#include <memory>
#include "utMath.h"
#include "FrameBuffer.h"


class Texture;
class VertexProcessor;
class FragmentProcessor;

class Renderer {
public:
    Renderer(int w, int h)
    : _width(w)
    , _height(h){
        _frameBuffer = std::make_shared<FrameBuffer>(_width, _height);
    }

    ~Renderer() {
        _frameBuffer = nullptr;
    }

    int getWidth() { return _width; }
    int getHeight() { return _height; }
    Matrix4f& getModelViewMatrix() { return _modelviewMat; }
    Matrix4f& getProjectionMatrix() { return _projectionMat; }
    Matrix4f& getViewportMatrix() { return _viewportMat; }

    void orbit(Vec2f delta);

    void dolly(float delta);

    void lookat(Vec3f& eye, Vec3f& target, Vec3f& up);

    void setupViewParams(float fov, float aspect, float zNear, float zFar);
    
    void viewport(int x, int y, int w, int h);

    void setupPipeline(std::shared_ptr<VertexProcessor> vp, std::shared_ptr<FragmentProcessor> fp);

    void clearDepthBuffer(float val);

    void clearColorBuffer(const Vec3f &color);
    
    void renderLine(int lineCnt,
                    Vec3f *vertices,
                    Vec3f *colors);

    void renderTriangle(int verticesCnt,
                        Vec3f *vertices,
                        Vec3f *colors,
                        Vec3f *normals,
                        Vec3f *textures,
                        std::shared_ptr<Texture> tex);

    void dumpRaw(uint8_t **data, int *sz);

private:
    int _width;
    int _height;

    Vec3f _eye;
    Vec3f _target;
    Vec3f _up;

    Matrix4f _modelviewMat;
    Matrix4f _projectionMat;
    Matrix4f _viewportMat;

    std::shared_ptr<FrameBuffer> _frameBuffer;
    std::shared_ptr<VertexProcessor> _vp;
    std::shared_ptr<FragmentProcessor> _fp;
};

#endif
