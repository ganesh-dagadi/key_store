cmake_minimum_required(VERSION 3.16)

project(key_store LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(network api)

add_executable(key_store main.cpp)

target_link_libraries(key_store PRIVATE network)
