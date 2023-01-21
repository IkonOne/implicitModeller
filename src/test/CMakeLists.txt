cmake_minimum_required(VERSION 3.16)
enable_testing()

add_executable(
    imk_test
    imk_test_main.cpp
)

target_link_libraries(
    imk_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(imk_test)