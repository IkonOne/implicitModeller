cmake_minimum_required(VERSION 3.16)

# sources for the library are added in the subdirectories
add_library(imk STATIC)
add_subdirectory(visualization)

target_link_libraries(imk
    glad
    glfw
    glm
)

target_include_directories(imk PUBLIC ../imk)