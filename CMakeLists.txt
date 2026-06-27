cmake_minimum_required(VERSION 3.16)

project(LEDMatrix C)

enable_testing()

add_subdirectory(lib)

option(BUILD_TESTS "build with tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(test)
endif()
