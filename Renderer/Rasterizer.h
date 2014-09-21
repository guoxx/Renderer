#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include <cassert>

class Vertex;
class FragmentProcessor;
class FrameBuffer;

class Rasterizer {
public:
    Rasterizer(int w, int h) {
        width = w;
        height = h;
    }

    void lineSegment(Vertex *vertices, FragmentProcessor &fp, FrameBuffer &fb);
    void triangle(Vertex *vertices, FragmentProcessor *fp, FrameBuffer *fbuffer);

private:
    int width;
    int height;
};

#endif
