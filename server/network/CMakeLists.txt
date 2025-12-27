add_library(network STATIC
    src/server.cpp
    src/parser.cpp
)

target_include_directories(network
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(network
    PRIVATE engine
)

target_compile_options(network
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)