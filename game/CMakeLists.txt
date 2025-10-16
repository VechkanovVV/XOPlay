add_library(GameService
    game_logic.cpp
    game_service.cpp
)

target_include_directories(GameService
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(GameService
    PUBLIC
        Database
)