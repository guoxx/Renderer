#ifndef __RENDERER__SCENEMESH__
#define __RENDERER__SCENEMESH__

#include <stdio.h>
#include <Scene.h>

class SceneMesh : public Scene{
public:
    SceneMesh();
    virtual ~SceneMesh();

    bool load(const char &file);
    void render(Renderer &rd);

private:
    int _pointCnt;
    int _polyCnt;
    float *_vertices;
    float *_texcoords;
    int *_triangles;
};

#endif /* defined(__RENDERER__SCENEMESH__) */
