cmake_minimum_required(VERSION 3.16)

add_subdirectory(imk)

if(IMK_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()