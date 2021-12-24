cmake_minimum_required(VERSION 3.16)

add_executable(shadertoy shadertoy.cpp
    PixelShaderView.cpp
    FileWatcher.cpp
)
target_link_libraries(shadertoy imk glad glfw)

target_compile_features(shadertoy PUBLIC cxx_std_17)