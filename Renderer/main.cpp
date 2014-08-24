#include <stdio.h>
#include "utMath.h"
#include "Renderer.h"

void scene0(Renderer *render) {
    Vec3f v[3] = {
        Vec3f(1, 0, 0), Vec3f(0, 2, 0), Vec3f(-1, 0, 0)
    };

    Vec3f c[3] = {
        Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1)
    };

    Vec3f n[3] = {
        Vec3f(0, 0, 1), Vec3f(0, 0, 1), Vec3f(0, 0, 1)
    };

    Vec3f v2[3] = {
        Vec3f(0, 2, 0), Vec3f(1, 0, 0), Vec3f(-1, 0, 0)
    };
    
    Vec3f c2[3] = {
        Vec3f(0.25f, 0.75f, 0.25f), Vec3f(0.75f, 0.25f, 0.25f), Vec3f(0.25f, 0.25f, 0.75f)
    };
    
    Vec3f n2[3] = {
        Vec3f(0, 0, -1), Vec3f(0, 0, -1), Vec3f(0, 0, -1)
    };

    render->renderTriangle(3, v, c, n, NULL);
    (void)v2;
    (void)n2;
    (void)c2;
//    render->renderTriangle(3, v2, c2, n2, NULL);
}

int main( int argc, char **argv ) {
    Renderer* render = new Renderer(512, 512);
    Vec3f eye = Vec3f(3.0f, 4.0f, 5.0f);
    Vec3f target = Vec3f(0.0f, 0.0f, 0.0f);
    Vec3f up = Vec3f(0.0f, 1.0f, 0.0f);
    render->lookat(eye, target, up);
    render->viewport(0, 0, 512, 512);
    render->setupViewParams(90.0f, 1.0f, 1.0f, 10.0f);

    Vec3f clearColor(0, 0, 0);
    render->clearColorBuffer(clearColor);
    scene0(render);

    render->dumpTga("/tmp/aa.tga");
    free(render);

    return 0;
}
