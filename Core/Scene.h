#ifndef __RENDERER__SCENE__
#define __RENDERER__SCENE__

#include <stdio.h>
#include <string>

class Renderer;

class Scene {
public:
    Scene() {
    }

    virtual ~Scene() {
    }

    virtual bool load(const char &file) = 0;
    virtual void render(Renderer &rd) = 0;

private:
};

#endif /* defined(__RENDERER__SCENE__) */
