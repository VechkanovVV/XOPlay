#pragma once
#include <string>

#include "game.h"

enum class GameState
{
    NotStarted,
    InProgress,
    WinFirst,
    WinSecond,
    Draw
};

enum class MoveStatus
{
    Ok,
    Win,
    Draw,
    InvalidColumn,
    ColumnFull,
    PlayerNotRegistered,
    NotPlayersTurn,
    GameFinished,
    UnknownError
};

struct MoveResult
{
    MoveStatus status;
    int col;
};

class GameService
{
   public:
    GameService() = default;

    Player addPlayer();
    GameState state() const;
    Player currentTurn() const;
    std::string field() const;
    MoveResult move(int col, Player player);

   private:
    Game game_;
    GameState state_{GameState::NotStarted};
    Player turn_{Player::First};
    bool twoPlayers_{false};
    bool applyWin(Player p);
};