#pragma once

#include <cassert>
#include <memory>
#include "Texture.h"

class Vertex;
class FragmentProcessor;
class FrameBuffer;
class Texture;

class Rasterizer
{
public:
    void setupTexture(std::shared_ptr<Texture> t)
	{
        _tex = t;
    }

    void line(Vertex *vertices, FragmentProcessor &fp, FrameBuffer &framebuffer);
    void triangle(Vertex *vertices, FragmentProcessor &fp, FrameBuffer &framebuffer);

private:

    std::shared_ptr<Texture> _tex;
};