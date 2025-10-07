#include "game_service.h"

Player GameService::addPlayer()
{
    Player p = game_.addPlayer();
    if (p == Player::Second)
    {
        twoPlayers_ = true;
        state_ = GameState::InProgress;
    }
    return p;
}

GameState GameService::state() const
{
    return state_;
}

Player GameService::currentTurn() const
{
    return turn_;
}

std::string GameService::field() const
{
    return game_.field();
}

MoveResult GameService::move(int col, Player player)
{
    try
    {
        if (!twoPlayers_)
        {
            return MoveResult{MoveStatus::PlayerNotRegistered, col};
        }

        if (player != turn_)
        {
            return MoveResult{MoveStatus::NotPlayersTurn, col};
        }
        auto result = game_.makeMoveReturnsWin(player, col);
        if (result)
        {
            applyWin(player);
            return MoveResult{MoveStatus::Win, col};
        }
        if (game_.freeCells() == 0)
        {
            state_ = GameState::Draw;
            return MoveResult{MoveStatus::Draw, col};
        }

        turn_ = (turn_ == Player::First) ? Player::Second : Player::First;
        return MoveResult{MoveStatus::Ok, col};
    }
    catch (const GameException& e)
    {
        switch (e.code())
        {
            case GameErrorCode::InvalidColumn:
                return MoveResult{MoveStatus::InvalidColumn, col};
            case GameErrorCode::ColumnFull:
                return MoveResult{MoveStatus::ColumnFull, col};
            case GameErrorCode::PlayerNotRegistered:
                return MoveResult{MoveStatus::PlayerNotRegistered, col};
            case GameErrorCode::TooManyPlayers:
                return MoveResult{MoveStatus::PlayerNotRegistered, col};
            default:
                return MoveResult{MoveStatus::UnknownError, col};
        }
    }
    catch (const std::exception&)
    {
        return MoveResult{MoveStatus::UnknownError, col};
    }
}

bool GameService::applyWin(Player p)
{
    state_ = (p == Player::First) ? GameState::WinFirst : GameState::WinSecond;
    return true;
}