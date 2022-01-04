cmake_minimum_required(VERSION 3.16)

# sources for the library are added in the subdirectories
add_library(imk STATIC)

target_sources(imk PRIVATE
    gl.cpp
    PixelShaderView.cpp

    utils/FileWatcher.cpp
)

target_link_libraries(imk
    fmt
    glad
    glfw
    glm
)

target_include_directories(imk PUBLIC ../imk)