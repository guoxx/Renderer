#include <stdio.h>
#include "utMath.h"
#include "Renderer.h"

int main( int argc, char **argv ) {
    Renderer* render = new Renderer(300, 600);
    render->viewport(50, 50, 200, 500);

    Vec3f clearColor(0, 1, 1);
    render->clearColorBuffer(clearColor);
    Vec4f color(1.0f, 0.0f, 0.0f, 1.0f);
    for (int i = 10; i < 290; ++i) {
        render->drawDot(i, 500, color);
    }

    render->dumpTga("/tmp/aa.tga");
    free(render);

    return 0;
}
