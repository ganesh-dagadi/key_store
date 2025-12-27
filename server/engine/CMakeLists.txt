add_library(engine STATIC
    src/engine.cpp
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(engine
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)