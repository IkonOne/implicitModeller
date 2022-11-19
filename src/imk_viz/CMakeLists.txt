cmake_minimum_required(VERSION 3.16)

add_library(imk_viz STATIC)
target_compile_features(imk_viz PUBLIC cxx_std_17)

target_sources(imk_viz PRIVATE
    gl.cpp
    PixelShaderView.cpp

    code_gen/genGLSL.cpp
)

target_include_directories(imk_viz PUBLIC ../imk_viz)

target_link_libraries(imk_viz
    imk
    fmt
    glad
    glfw
)