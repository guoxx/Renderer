#ifndef __RENDERER__FRAMEBUFFER__
#define __RENDERER__FRAMEBUFFER__

#include "utMath.h"
#include <cassert>

class FrameBuffer {
public:
    FrameBuffer () {
        assert(false);
    }

    FrameBuffer(int w, int h) {
        width = w;
        height = h;
        colorBuffer = new Vec3f[width * height];
        depthBuffer = new float[width * height];
    }

    ~FrameBuffer() {
        delete [] colorBuffer;
        delete [] depthBuffer;
    }

    void setColorBuffer(int x, int y, const Vec3f &val) {
        colorBuffer[x + y * width] = val;
    }

    Vec3f getColorBuffer(int x, int y) {
        return colorBuffer[x + y * width];
    }

    void setDepthBuffer(int x, int y, float val) {
        depthBuffer[x + y * width] = val;
    }
    
    float getDepthBuffer(int x, int y) {
        return depthBuffer[x + y * width];
    }

private:
    int width;
    int height;
    Vec3f *colorBuffer;
    float *depthBuffer;
};

#endif /* defined(__RENDERER__FRAMEBUFFER__) */
