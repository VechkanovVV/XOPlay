#include "game_service.h"

#include <gtest/gtest.h>

TEST(GameServiceTest, AddPlayersAndStart)
{
    GameService svc;
    EXPECT_EQ(svc.state(), GameState::NotStarted);
    EXPECT_EQ(svc.addPlayer(), Player::First);
    EXPECT_EQ(svc.addPlayer(), Player::Second);
    EXPECT_EQ(svc.state(), GameState::InProgress);
}

TEST(GameServiceTest, NotPlayersTurn)
{
    GameService svc;
    svc.addPlayer();
    svc.addPlayer();
    auto res = svc.move(0, Player::Second);
    EXPECT_EQ(res.status, MoveStatus::NotPlayersTurn);
}

TEST(GameServiceTest, InvalidColumn)
{
    GameService svc;
    svc.addPlayer();
    svc.addPlayer();
    auto res = svc.move(-1, Player::First);
    EXPECT_EQ(res.status, MoveStatus::InvalidColumn);
}

TEST(GameServiceTest, HorizontalWinForFirst)
{
    GameService svc;
    svc.addPlayer();
    svc.addPlayer();

    MoveResult r;

    r = svc.move(0, Player::First);
    EXPECT_EQ(r.status, MoveStatus::Ok);
    r = svc.move(6, Player::Second);
    EXPECT_EQ(r.status, MoveStatus::Ok);

    r = svc.move(1, Player::First);
    EXPECT_EQ(r.status, MoveStatus::Ok);
    r = svc.move(6, Player::Second);
    EXPECT_EQ(r.status, MoveStatus::Ok);

    r = svc.move(2, Player::First);
    EXPECT_EQ(r.status, MoveStatus::Ok);
    r = svc.move(6, Player::Second);
    EXPECT_EQ(r.status, MoveStatus::Ok);

    r = svc.move(3, Player::First);
    EXPECT_EQ(r.status, MoveStatus::Win);
    EXPECT_EQ(svc.state(), GameState::WinFirst);
}