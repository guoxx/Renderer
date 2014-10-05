#ifndef __RENDERER__FRAMEBUFFER__
#define __RENDERER__FRAMEBUFFER__

#include "utMath.h"
#include <cassert>

class FrameBuffer {
public:
    FrameBuffer(int w, int h) {
        _width = w;
        _height = h;
        _colorBuffer = new Vec3f[_width * _height];
        _depthBuffer = new float[_width * _height];
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
    Vec3f *_colorBuffer;
    float *_depthBuffer;
};

#endif /* defined(__RENDERER__FRAMEBUFFER__) */
