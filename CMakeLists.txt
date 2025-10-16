cmake_minimum_required(VERSION 3.5)
project(XOPlay LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(game)
add_subdirectory(thread_pool)
add_subdirectory(config)
add_subdirectory(db)
add_subdirectory(task_manager)

add_executable(xoplay src/main.cpp)
target_link_libraries(xoplay PRIVATE GameService mbot TaskManager config Database)

if (XOPLAY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()