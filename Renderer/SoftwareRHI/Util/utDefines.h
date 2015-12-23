#pragma once

#if defined __APPLE__
    #define GLFW_EXPOSE_NATIVE_COCOA
    #define GLFW_EXPOSE_NATIVE_NSGL
#elif defined _WIN32 || defined _WIN64
    #define GLFW_EXPOSE_NATIVE_WIN32
    #define GLFW_EXPOSE_NATIVE_WGL
#else
    #error "Unsupported platform"
#endif

#if defined __APPLE__
    #define GL_BGR_EXT GL_BGR
#endif