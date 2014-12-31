#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "utMath.h"
#include "Renderer.h"
#include "Texture.h"
#include "SceneMesh.h"
#include "ConstColorVP.h"
#include "TrivialColorFP.h"


GLuint texId = 0;
int screenWidth = 512;
int screenHeight = 512;

static bool _initialized = false;
std::shared_ptr<Renderer> renderer;
std::shared_ptr<Scene> scene;

Vec2f lastMousePoint;
static bool leftButtonPressed = false;
static bool rightButtonPressed = false;

void _initializeEnv() {
    if (_initialized)
        return;

    renderer = std::make_shared<Renderer>(screenWidth, screenHeight);
    renderer->setupPipeline(std::make_shared<ConstColorVP>(), std::make_shared<TrivialColorFP>());
    scene = std::make_shared<SceneMesh>();
    const char *file = "../Resources/bunny500.msh";
    scene->load(*file);
    scene->setup(*renderer);
}

void _finalizeEnv() {
    scene = nullptr;
    renderer = nullptr;
}

void redraw() {
    scene->update(*renderer);
    scene->render(*renderer);

    uint8_t *data;
    renderer->dumpRaw(&data, NULL);

    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, screenWidth, screenHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
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

static Vec2f _windowToViewport(Vec2f p) {
    return Vec2f(p.x / screenHeight, (screenHeight - p.y) / screenHeight);
}

static void _keyPressListener(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void _mouseClickListener(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        leftButtonPressed = (action == GLFW_PRESS);
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        rightButtonPressed = (action == GLFW_PRESS);

    if (leftButtonPressed || rightButtonPressed) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastMousePoint = _windowToViewport(Vec2f(xpos, ypos));
    }
}

static void _mouseMoveListener(GLFWwindow *window, double x, double y) {
    if (!leftButtonPressed && !rightButtonPressed) {
        return;
    }

    Vec2f mouseVec = _windowToViewport(Vec2f(x, y));
    Vec2f mouseDelta = mouseVec - lastMousePoint;

    if (leftButtonPressed)
        renderer->orbit(mouseDelta);
    else if (rightButtonPressed)
        renderer->dolly(mouseDelta.y);

    lastMousePoint = mouseVec;
}

int _main(int argc, char **argv) {
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
    glfwSetMouseButtonCallback(window, _mouseClickListener);
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

int main(int argc, char **argv) {
    _initializeEnv();
    int ret = _main(argc, argv);
    _finalizeEnv();
    return ret;
}