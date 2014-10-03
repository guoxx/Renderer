#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL

#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "utMath.h"
#include "Renderer.h"
#include "Texture.h"

GLuint texId = 0;
int screenWidth = 512;
int screenHeight = 512;

void scene0(Renderer *render) {

    {
        Vec3f v[3] = {
            Vec3f(-1, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 2, 0)
        };
        
        Vec3f c[3] = {
            Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1)
        };
        
        Vec3f n[3] = {
            Vec3f(0, 0, 1), Vec3f(0, 0, 1), Vec3f(0, 0, 1)
        };
        
        Vec3f t[3] = {
            Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0)
        };
        
//        Vec3f v2[3] = {
//            Vec3f(0, 2, 0), Vec3f(1, 0, 0), Vec3f(-1, 0, 0)
//        };
//        
//        Vec3f c2[3] = {
//            Vec3f(0.25f, 0.75f, 0.25f), Vec3f(0.75f, 0.25f, 0.25f), Vec3f(0.25f, 0.25f, 0.75f)
//        };
//        
//        Vec3f n2[3] = {
//            Vec3f(0, 0, -1), Vec3f(0, 0, -1), Vec3f(0, 0, -1)
//        };
        
        Texture *tex = new Texture();
        const char *tgaFile = "/Users/ben/Downloads/aaa.tga";
        tex->loadTga(*tgaFile);
        render->renderTriangle(3, v, c, n, t, tex);
    }

    {
        Vec3f v[3] = {
            Vec3f(0, 2, 0), Vec3f(1, 0, 0), Vec3f(2, 2, 0)
        };

        Vec3f c[3] = {
            Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1)
        };
        
        Vec3f n[3] = {
            Vec3f(0, 0, 1), Vec3f(0, 0, 1), Vec3f(0, 0, 1)
        };
        
        Vec3f t[3] = {
            Vec3f(0, 1, 0), Vec3f(1, 0, 0), Vec3f(1, 1, 0)
        };
        
        //        Vec3f v2[3] = {
        //            Vec3f(0, 2, 0), Vec3f(1, 0, 0), Vec3f(-1, 0, 0)
        //        };
        //
        //        Vec3f c2[3] = {
        //            Vec3f(0.25f, 0.75f, 0.25f), Vec3f(0.75f, 0.25f, 0.25f), Vec3f(0.25f, 0.25f, 0.75f)
        //        };
        //
        //        Vec3f n2[3] = {
        //            Vec3f(0, 0, -1), Vec3f(0, 0, -1), Vec3f(0, 0, -1)
        //        };
        
        Texture *tex = new Texture();
        const char *tgaFile = "/Users/ben/Downloads/aaa.tga";
        tex->loadTga(*tgaFile);
        render->renderTriangle(3, v, c, n, t, tex);
    }

//    {
//        Vec3f v[2];
//        Vec3f c[3] = {
//            Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1)
//        };
//        v[0] = Vec3f(1, 0, 0);
//        v[1] = Vec3f(0, 2, 0);
//        render->renderLine(2, v, c);
//        v[0] = Vec3f(0, 2, 0);
//        v[1] = Vec3f(-1, 0, 0);
//        render->renderLine(2, v, c);
//        v[0] = Vec3f(1, 0, 0);
//        v[1] =  Vec3f(-1, 0, 0);
//        render->renderLine(2, v, c);
//        v[0] = Vec3f(1, 0, -3);
//        v[1] =  Vec3f(-1, 0, 0);
//        render->renderLine(2, v, c + 1);
//    }

//    render->renderTriangle(3, v2, c2, n2, NULL);
}

void redraw() {
    static Renderer* render = NULL;
    if (render == NULL) {
        render = new Renderer(screenWidth, screenHeight);
    }

    Vec3f eye = Vec3f(3.0f, 4.0f, 5.0f);
    Vec3f target = Vec3f(0.0f, 0.0f, 0.0f);
    Vec3f up = Vec3f(0.0f, 1.0f, 0.0f);
    render->lookat(eye, target, up);
    render->viewport(0, 0, screenWidth, screenHeight);
    render->setupViewParams(90.0f, 1.0f, 1.0f, 10.0f);

    Vec3f clearColor(0, 0, 0);
    render->clearColorBuffer(clearColor);
    scene0(render);

    uint8_t *data;
    render->dumpRaw(&data, NULL);

    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, screenWidth, screenHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    free(data);
}

void render() {
    redraw();

    GLFWwindow *window = glfwGetCurrentContext();
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable( GL_TEXTURE_2D );
    glBindTexture(GL_TEXTURE_2D, texId);

    glBegin( GL_QUADS );
    glTexCoord2d(0.0,0.0); glVertex3f(-1.0, -1.0, 0.0);
    glTexCoord2d(1.0,0.0); glVertex3f(1.0,-1.0, 0.0);
    glTexCoord2d(1.0,1.0); glVertex3f(1.0,1.0, 0.0);
    glTexCoord2d(0.0,1.0); glVertex3f(-1.0,1.0, 0.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glfwSwapBuffers(window);
}

void reshape(GLFWwindow* window, int w, int h) {
    // TODO: retina display support
    glViewport( 0, 0, (GLsizei)w * 2, (GLsizei)h * 2 );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    float ratio = w / (float)h;
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
}

static void _keyPressListener(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void _mouseMoveListener(GLFWwindow *window, double x, double y) {
    
}

int main(int argc, char **argv) {
	if (!glfwInit()) {
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Renderer", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    reshape(window, screenWidth, screenHeight);

    glfwSetKeyCallback(window, _keyPressListener);
    glfwSetCursorPosCallback(window, _mouseMoveListener);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glGenTextures(1, &texId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        render();
    }

    glfwTerminate();
    return 0;
}