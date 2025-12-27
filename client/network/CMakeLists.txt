cmake_minimum_required(VERSION 3.16)

add_library(network STATIC
        src/client.cpp
)

target_include_directories(network
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(network
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
