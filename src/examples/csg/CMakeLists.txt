cmake_minimum_required(VERSION 3.16)

add_executable(csg csg.cpp)
target_link_libraries(csg imk imk_viz)