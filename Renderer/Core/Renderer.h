#pragma once

#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstring>
#include <memory>
#include "utMath.h"
#include "FrameBuffer.h"
#include "RenderEnum.h"
#include "RenderCommand.h"
#include "RenderProgram.h"


class Texture;
class VertexProcessor;
class FragmentProcessor;


class Renderer {
public:
    Renderer(int w, int h)
		: _width{w}
		, _height{h}
		, _frameBuffer{std::make_shared<FrameBuffer>(w, h)}
	{
    }

	Renderer(const Renderer&) = delete;
	Renderer& operator= (const Renderer&) = delete;
	Renderer(const Renderer&&) = delete;
	Renderer& operator= (const Renderer&&) = delete;

    ~Renderer() = default;

    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    const Matrix4f& getModelViewMatrix() const { return _modelviewMat; }
    const Matrix4f& getProjectionMatrix() const { return _projectionMat; }
    const Matrix4f& getViewportMatrix() const { return _viewportMat; }

    void orbit(Vec2f delta);

    void dolly(float delta);

    void lookat(Vec3f& eye, Vec3f& target, Vec3f& up);

    void setupViewParams(float fov, float aspect, float zNear, float zFar);
    
    void viewport(int x, int y, int w, int h);

	template<typename VP, typename FP>
	void setupPipeline()
	{
		_vp = std::make_shared<VP>();
		_fp = std::make_shared<FP>();
	}

    void clearDepthBuffer(float val);

    void clearColorBuffer(const Vec3f &color);
    
	void useProgram(RenderProgram& program);

	void vertexAttribPointer(uint32_t index, int size, void* data);

	void drawArray(RenderDrawMode mode, std::shared_ptr<std::vector<float>>& indices);

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

	std::vector<RenderCommand> _cmds;
};