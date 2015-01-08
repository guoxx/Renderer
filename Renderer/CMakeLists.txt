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
        ${CMAKE_CURRENT_SOURCE_DIR}/../Libs/glfw/include
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

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    IF(MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSECURE_SCL=0")        
    ENDIF(MSVC)
    FIND_PACKAGE(OpenGL REQUIRED)
    target_link_libraries(renderer ${OPENGL_gl_LIBRARY} glfw)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
    FIND_LIBRARY(GLFW_LIBRARY glfw)
    target_link_libraries(renderer ${GLFW_LIBRARY} ${OPENGL_LIBRARY})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")