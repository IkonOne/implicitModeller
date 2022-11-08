cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wpedantic -Werror)
add_compile_options(-std=c++17)
add_compile_options(-march=x86-64 -O3)

# sources for the library are added in the subdirectories
add_library(imk STATIC)

target_sources(imk PRIVATE
    gl.cpp
    PixelShaderView.cpp

    code_gen/genGLSL.cpp

    csg/csg_factory.cpp
    csg/csg_node.cpp

    utils/FileWatcher.cpp
)

target_link_libraries(imk
    fmt
    glad
    glfw
    glm
    stdc++fs # FIXME: linux only???
)

target_include_directories(imk PUBLIC ../imk)