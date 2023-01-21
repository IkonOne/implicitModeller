cmake_minimum_required(VERSION 3.16)


add_subdirectory(imk)
target_compile_features(imk PUBLIC cxx_std_17)

if(IMK_BUILD_VIZ OR IMK_BUILD_EXAMPLES)
    add_subdirectory(imk_viz)
endif()

if(IMK_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(IMK_BUILD_TESTS)
    add_subdirectory(test)
endif()