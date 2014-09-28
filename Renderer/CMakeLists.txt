
# CMAKE_BINARY_DIR is used for inclusion of automatically generated files
include_directories(. /usr/local/include ${CMAKE_BINARY_DIR})


# set(RENDERER_SOURCES 
#     )

FILE(GLOB_RECURSE TARGET_SRC  "*.cpp")
FILE(GLOB_RECURSE TARGET_INC  "*.h")
SET(TARGET_SRC ${TARGET_SRC}  ${TARGET_INC})

# aux_source_directory(./ RENDERER_SOURCES)

add_definitions(-DCMAKE )

add_executable(renderer 
    ${TARGET_SRC}
    )

FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
FIND_LIBRARY(GLFW_LIBRARY GLFW)
target_link_libraries(renderer ${GLFW_LIBRARY} ${OPENGL_LIBRARY})
