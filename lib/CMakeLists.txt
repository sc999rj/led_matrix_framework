cmake_minimum_required(VERSION 3.16)

project(led_matrix C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Create the library
add_library(led_matrix STATIC
    led_matrix_api.c
)

# Make header visible to users of the library
target_include_directories(led_matrix
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Recommended warnings for driver code
# target_compile_options(led_matrix PRIVATE
#     -Wall
#     -Wextra
#     -Wpedantic
# )