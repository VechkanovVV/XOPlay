#pragma once

#include <string>
#include <vector>

#include "game_exception.h"

enum class Player
{
    First,
    Second
};

enum class GridNumbers
{
    Row = 6,
    Col = 7,
    MaxSteps = 3
};

class Game final
{
   public:
    Game();
    Player addPlayer();
    bool makeMoveReturnsWin(Player p, int col);
    int freeCells() const;
    std::string field() const;

   private:
    bool winner(int x, int y) const;
    int playersCount_;
    int freeCellsCount_;
    const int row_;
    const int col_;
    std::vector<std::vector<char>> grid_;
};