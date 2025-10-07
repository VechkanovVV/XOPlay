cmake_minimum_required(VERSION 3.5)

project(XOPlay LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(game)

add_executable(xoplay
    src/main.cpp
)

target_link_libraries(xoplay PRIVATE GameService)

add_subdirectory(tests)