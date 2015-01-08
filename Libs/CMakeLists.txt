
find_library(GLFW_LIBRARY "glfw3")
if (NOT GLFW_LIBRARY)
    add_subdirectory(glfw)
endif (NOT GLFW_LIBRARY)
