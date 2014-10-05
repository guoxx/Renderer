cmake_minimum_required(VERSION 2.8.0)

# Project details.
project(Renderer)

SET(${PROJECT_NAME}_MAJOR_VERSION 0)
SET(${PROJECT_NAME}_MINOR_VERSION 1)
SET(${PROJECT_NAME}_PATCH_LEVEL 0)

# Look for required GLFW library
find_package(GLFW)

# Set binaries output folder.
SET(RENDERER_OUTPUT_PATH_PREFIX "${PROJECT_BINARY_DIR}/Binaries")
SET(RENDERER_OUTPUT_PATH_SUFFIX "")
IF(${CMAKE_CFG_INTDIR} STREQUAL ".")
    SET(RENDERER_OUTPUT_PATH_SUFFIX ${CMAKE_BUILD_TYPE})
    IF(NOT RENDERER_OUTPUT_PATH_SUFFIX)
        SET(RENDERER_OUTPUT_PATH_SUFFIX "Release")
    ENDIF(NOT RENDERER_OUTPUT_PATH_SUFFIX)
ENDIF(${CMAKE_CFG_INTDIR} STREQUAL ".")

# c++11 support
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

# definitions
add_definitions(-DCMAKE)

# 
file(GLOB_RECURSE UTIL_SRCS
        RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Util/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Util/*.h)
file(GLOB_RECURSE CORE_SRCS
        RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.h)
file(GLOB_RECURSE VERTEX_SRCS
        RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Vertex/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Vertex/*.h)
file(GLOB_RECURSE FRAGMENT_SRCS
        RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Fragment/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Fragment/*.h)
file(GLOB_RECURSE SCENE_SRCS
        RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Scene/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Scene/*.h)

source_group(Util FILES ${UTIL_SRCS})
source_group(Core FILES ${CORE_SRCS})
source_group(Vertex FILES ${VERTEX_SRCS})
source_group(Fragment FILES ${FRAGMENT_SRCS})
source_group(Scene FILES ${SCENE_SRCS})

# CMAKE_BINARY_DIR is used for inclusion of automatically generated files
include_directories(
        /usr/local/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Util
        ${CMAKE_CURRENT_SOURCE_DIR}/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/Vertex
        ${CMAKE_CURRENT_SOURCE_DIR}/Fragment
        ${CMAKE_CURRENT_SOURCE_DIR}/Scene
        ${CMAKE_BINARY_DIR}
    )

add_executable(renderer
        ${UTIL_SRCS}
        ${CORE_SRCS}
        ${VERTEX_SRCS}
        ${FRAGMENT_SRCS}
        ${SCENE_SRCS}
    )

FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
FIND_LIBRARY(GLFW_LIBRARY GLFW)
target_link_libraries(renderer ${GLFW_LIBRARY} ${OPENGL_LIBRARY})