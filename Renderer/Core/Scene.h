#pragma once

#include <stdio.h>
#include <string>

class Renderer;

class Scene {
public:
    virtual ~Scene() {}

    virtual void setup(Renderer &renderer) = 0;
    virtual void update(Renderer &renderer) = 0;
    virtual void render(Renderer &renderer) = 0;
};