add_library(network STATIC
    src/server.cpp
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