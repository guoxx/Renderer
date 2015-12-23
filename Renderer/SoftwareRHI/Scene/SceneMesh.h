#pragma once

#include <iostream>
#include <memory>
#include "Scene.h"

class SceneMesh : public Scene{
public:
	SceneMesh(std::string& file);
    ~SceneMesh() override final = default;

    void setup(Renderer &renderer);
    void update(Renderer &renderer);
    void render(Renderer &renderer);

private:
    int _pointCnt;
    int _polyCnt;
	std::unique_ptr<float[]> _verticesPtr;
    std::unique_ptr<float[]> _texcoordsPtr;
    std::unique_ptr<int[]> _trianglesPtr;
};