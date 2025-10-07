#include "game.h"

#include <string>

namespace
{
constexpr char kEmptyCell = ' ';
constexpr char kFirstToken = 'X';
constexpr char kSecondToken = 'O';
constexpr int kWinOffset = static_cast<int>(GridNumbers::MaxSteps);
}  // namespace

Game::Game()
    : playersCount_(0),
      freeCellsCount_(0),
      row_(static_cast<int>(GridNumbers::Row)),
      col_(static_cast<int>(GridNumbers::Col)),
      grid_(row_, std::vector<char>(col_, kEmptyCell))
{
    freeCellsCount_ = row_ * col_;
}

Player Game::addPlayer()
{
    if (playersCount_ == 0)
    {
        playersCount_++;
        return Player::First;
    }
    else if (playersCount_ == 1)
    {
        playersCount_++;
        return Player::Second;
    }
    throw GameException(GameErrorCode::TooManyPlayers, "Too many players");
}

bool Game::makeMoveReturnsWin(Player p, int col)
{
    if (col < 0 || col >= col_) throw GameException(GameErrorCode::InvalidColumn, "Invalid column index");

    if ((p == Player::Second && playersCount_ < 2) || (p == Player::First && playersCount_ < 1))
        throw GameException(GameErrorCode::PlayerNotRegistered, "Player not registered");

    int index = row_ - 1;

    if (p == Player::First)
    {
        while (index >= 0)
        {
            if (grid_[index][col] == kEmptyCell)
            {
                grid_[index][col] = kFirstToken;
                freeCellsCount_--;
                return winner(index, col);
            }
            index--;
        }
        throw GameException(GameErrorCode::ColumnFull, "Column is full");
    }

    while (index >= 0)
    {
        if (grid_[index][col] == kEmptyCell)
        {
            grid_[index][col] = kSecondToken;
            freeCellsCount_--;
            return winner(index, col);
        }
        index--;
    }
    throw GameException(GameErrorCode::ColumnFull, "Column is full");
}

int Game::freeCells() const
{
    return freeCellsCount_;
}

std::string Game::field() const
{
    std::string f;
    f.reserve((col_ * 2 + 2) * row_);
    for (int i = 0; i < row_; ++i)
    {
        f.push_back('|');
        for (int j = 0; j < col_; ++j)
        {
            f.push_back(grid_[i][j]);
            f.push_back('|');
        }
        f.push_back('\n');
    }
    return f;
}

bool Game::winner(int x, int y) const
{
    char tar = grid_[x][y];
    if (tar == kEmptyCell) return false;

    auto count_dir = [&](int dx, int dy)
    {
        int cnt = 0;
        int rx = x + dx;
        int ry = y + dy;
        while (rx >= 0 && rx < row_ && ry >= 0 && ry < col_ && grid_[rx][ry] == tar)
        {
            ++cnt;
            rx += dx;
            ry += dy;
        }
        return cnt;
    };

    const int need = kWinOffset + 1;

    if (1 + count_dir(0, 1) + count_dir(0, -1) >= need) return true;
    if (1 + count_dir(1, 0) + count_dir(-1, 0) >= need) return true;
    if (1 + count_dir(1, 1) + count_dir(-1, -1) >= need) return true;
    if (1 + count_dir(1, -1) + count_dir(-1, 1) >= need) return true;

    return false;
}