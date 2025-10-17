include(FetchContent)

FetchContent_Declare(
  tgbot
  GIT_REPOSITORY https://github.com/reo7sp/tgbot-cpp.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(tgbot)

add_library(mbot
    bot.cpp
)

target_include_directories(mbot
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${tgbot_SOURCE_DIR}/include
)

target_link_libraries(mbot
    PUBLIC
        GameService
        TaskManager
        config
        Database
        TgBot
)