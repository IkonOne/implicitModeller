cmake_minimum_required(VERSION 3.16)

project(imk)
set(CMAKE_CXX_STANDARD 17)

option(IMK_BUILD_BIZ "Build visualization library..." ON)
option(IMK_BUILD_EXAMPLES "Build examples..." ON)
option(IMK_BUILD_TESTS "Build tests..." ON)

if(IMK_BUILD_TESTS)

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(3rdparty)
add_subdirectory(src)