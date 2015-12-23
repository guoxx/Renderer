#pragma once

#include "utMath.h"
#include <cassert>

class FrameBuffer
{
public:
    FrameBuffer(int w, int h)
	try
		: _width{w}
		, _height{h}
		, _colorBuffer{new Vec3f[w * h]}
		, _depthBuffer{new float[w * h]}
	{
    }
	catch(std::bad_alloc&)
	{
		if (_colorBuffer != nullptr) { delete[] _colorBuffer; }
		if (_depthBuffer != nullptr) { delete[] _depthBuffer; }
	}

    ~FrameBuffer() {
        delete [] _colorBuffer;
        delete [] _depthBuffer;
    }

    void setColorBuffer(int x, int y, const Vec3f &val) {
        _colorBuffer[x + y * _width] = val;
    }

    Vec3f getColorBuffer(int x, int y) {
        return _colorBuffer[x + y * _width];
    }

    void setDepthBuffer(int x, int y, float val) {
        _depthBuffer[x + y * _width] = val;
    }
    
    float getDepthBuffer(int x, int y) {
        return _depthBuffer[x + y * _width];
    }

private:
    int _width;
    int _height;
    Vec3f* _colorBuffer;
    float* _depthBuffer;
};