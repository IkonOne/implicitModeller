cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wpedantic -Werror)
add_compile_options(-std=c++17)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-march=x86-64 -O3)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # add_compile_options(-march=x86-64 -03)
endif()

# sources for the library are added in the subdirectories
add_library(imk STATIC)

target_sources(imk PRIVATE
    csg/csg_factory.cpp
    csg/csg_node.cpp

    utils/FileWatcher.cpp
)

target_link_libraries(imk
    glm
    stdc++fs # FIXME: linux only???
)

target_include_directories(imk PUBLIC ../imk)