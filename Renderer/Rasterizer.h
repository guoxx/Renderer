#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include <cassert>
#include "Texture.h"

struct Vertex;
class FragmentProcessor;
class FrameBuffer;
class Texture;

class Rasterizer {
public:
    Rasterizer()
    : _tex(nullptr) {
        
    }

    ~Rasterizer() {
        if (_tex) {
            delete _tex;
            _tex = nullptr;
        }
    }

    void setupTexture(Texture *t) {
        if (_tex != nullptr) {
            delete _tex;
        }
        _tex = t;
    }
    
    void lineSegment(Vertex *vertices, FragmentProcessor &fp, FrameBuffer &fb);
    void triangle(Vertex *vertices, FragmentProcessor *fp, FrameBuffer *fbuffer);

private:
    Texture *_tex;
};

#endif
