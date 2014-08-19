#ifndef __RENDERER_RENDERABLEPROTOCOL_H__
#define __RENDERER_RENDERABLEPROTOCOL_H__

class Renderer;

class RenderableProtocol {
    friend Renderer;
public:
    virtual void render(Renderer *rd);
};

#endif
