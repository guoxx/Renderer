#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include <cassert>
#include <memory>
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
        _tex = nullptr;
    }

    void setupTexture(std::shared_ptr<Texture> t) {
        _tex = t;
    }

    void line(Vertex *vertices, FragmentProcessor &fp, FrameBuffer &framebuffer);
    void triangle(Vertex *vertices, FragmentProcessor &fp, FrameBuffer &framebuffer);

private:

    std::shared_ptr<Texture> _tex;
};

#endif
