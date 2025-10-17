#include "game_logic.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

GameLogic::GameLogic(int id, int64_t player1, int64_t player2)
    : id_(id), player1_id_(player1), player2_id_(player2), current_player_(player1), moves_count_(0)
{
    for (auto& row : board_)
    {
        row.fill('.');
    }
}

bool GameLogic::makeMove(int64_t player_id, int column)
{
    if (player_id != current_player_)
    {
        return false;
    }

    if (column < 0 || column >= COLS)
    {
        return false;
    }

    int row = getNextFreeRow(column);
    if (row < 0)
    {
        return false;
    }

    board_[row][column] = getPlayerSymbol(player_id);
    moves_count_++;

    current_player_ = (current_player_ == player1_id_) ? player2_id_ : player1_id_;

    return true;
}

void GameLogic::determineCurrentPlayer()
{
    current_player_ = (moves_count_ % 2 == 0) ? player1_id_ : player2_id_;
}

bool GameLogic::isWinner(int64_t player_id) const
{
    char symbol = getPlayerSymbol(player_id);

    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            if (board_[row][col] == symbol)
            {
                if (checkWin(row, col, player_id))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool GameLogic::isDraw() const
{
    return moves_count_ == ROWS * COLS;
}

char GameLogic::getCell(int row, int col) const
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
    {
        throw std::out_of_range("Cell coordinates out of range");
    }
    return board_[row][col];
}

std::string GameLogic::getBoardString() const
{
    std::string result;
    result.reserve(ROWS * COLS);

    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            result += board_[row][col];
        }
    }

    return result;
}

void GameLogic::setBoardFromString(const std::string& board)
{
    if (board.length() != ROWS * COLS)
    {
        throw std::invalid_argument("Invalid board string length");
    }

    moves_count_ = 0;

    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            int index = row * COLS + col;
            char cell = board[index];
            board_[row][col] = cell;

            if (cell != '.')
            {
                moves_count_++;
            }
        }
    }

    determineCurrentPlayer();
}

std::string GameLogic::renderBoard() const
{
    std::stringstream ss;
    ss << "   ";
    for (int col = 0; col < COLS; ++col)
    {
        ss << (col + 1) << "  ";
    }
    ss << "\n";

    ss << "  +";
    for (int col = 0; col < COLS; ++col)
    {
        ss << "---";
    }
    ss << "+\n";

    for (int row = ROWS - 1; row >= 0; --row)
    {
        ss << "  |";
        for (int col = 0; col < COLS; ++col)
        {
            ss << ' ' << board_[row][col] << ' ';
        }
        ss << "|\n";
    }

    ss << "  +";
    for (int col = 0; col < COLS; ++col)
    {
        ss << "---";
    }
    ss << "+";

    return ss.str();
}

bool GameLogic::checkWin(int row, int col, int64_t player_id) const
{
    char symbol = getPlayerSymbol(player_id);

    return checkLine(row, col, 0, 1, symbol) || checkLine(row, col, 1, 0, symbol) ||
           checkLine(row, col, 1, 1, symbol) || checkLine(row, col, 1, -1, symbol);
}

bool GameLogic::checkLine(int row, int col, int dr, int dc, char symbol) const
{
    int count = 0;

    for (int i = -3; i <= 3; ++i)
    {
        int r = row + i * dr;
        int c = col + i * dc;

        if (r < 0 || r >= ROWS || c < 0 || c >= COLS)
        {
            continue;
        }

        if (board_[r][c] == symbol)
        {
            count++;
            if (count >= WIN_COUNT)
            {
                return true;
            }
        }
        else
        {
            count = 0;
        }
    }

    return false;
}

int GameLogic::getNextFreeRow(int column) const
{
    for (int row = 0; row < ROWS; ++row)
    {
        if (board_[row][column] == '.')
        {
            return row;
        }
    }
    return -1;
}

char GameLogic::getPlayerSymbol(int64_t player_id) const
{
    if (player_id == player1_id_)
    {
        return 'x';
    }
    else if (player_id == player2_id_)
    {
        return 'o';
    }
    else
    {
        throw std::invalid_argument("Invalid player ID");
    }
}