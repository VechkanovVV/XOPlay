include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(game_service_test
    game/game_service_test.cpp
)

target_include_directories(game_service_test PRIVATE
    ${CMAKE_SOURCE_DIR}/game
)

target_link_libraries(game_service_test PRIVATE GTest::gtest_main GameService)

add_test(NAME game_service COMMAND game_service_test)
