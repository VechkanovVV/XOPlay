add_library(TaskManager
    task_manager.cpp
)

target_include_directories(TaskManager
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/game
        ${CMAKE_SOURCE_DIR}/db
        ${CMAKE_SOURCE_DIR}/thread_pool
)

target_link_libraries(TaskManager PRIVATE ThreadPool)