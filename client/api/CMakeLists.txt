cmake_minimum_required(VERSION 3.16)

add_library(api STATIC
        src/api.cpp
)

target_include_directories(api
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(api
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
