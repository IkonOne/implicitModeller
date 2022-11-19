cmake_minimum_required(VERSION 3.16)

project(imk)

option(IMK_BUILD_BIZ "Build visualization library..." ON)
option(IMK_BUILD_EXAMPLES "Build examples..." ON)

add_subdirectory(3rdparty)
add_subdirectory(src)