#pragma once
#include <stdexcept>
#include <string>

enum class GameErrorCode
{
    InvalidColumn,
    ColumnFull,
    PlayerNotRegistered,
    TooManyPlayers,
    InternalError
};

class GameException : public std::runtime_error
{
   public:
    inline GameException(GameErrorCode code, const std::string &msg) : std::runtime_error(msg), code_(code) {}
    inline GameErrorCode code() const noexcept { return code_; }

   private:
    GameErrorCode code_;
};