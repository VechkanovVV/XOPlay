#pragma once

#include <array>
#include <string>
#include <vector>

class GameLogic
{
   public:
    static constexpr int ROWS = 6;
    static constexpr int COLS = 7;
    static constexpr int WIN_COUNT = 4;

    GameLogic(int id, int64_t player1, int64_t player2);

    bool makeMove(int64_t player_id, int column);
    bool isWinner(int64_t player_id) const;
    bool isDraw() const;
    char getCell(int row, int col) const;

    int getId() const { return id_; }
    int64_t getPlayer1() const { return player1_id_; }
    int64_t getPlayer2() const { return player2_id_; }
    int64_t getCurrentPlayer() const { return current_player_; }

    std::string getBoardString() const;
    void setBoardFromString(const std::string& board);

    std::string renderBoard() const;

    void determineCurrentPlayer();

   private:
    int id_;
    int64_t player1_id_;
    int64_t player2_id_;
    int64_t current_player_;
    std::array<std::array<char, COLS>, ROWS> board_;
    int moves_count_ = 0;

    bool checkWin(int row, int col, int64_t player_id) const;
    bool checkLine(int row, int col, int dr, int dc, char symbol) const;
    int getNextFreeRow(int column) const;
    char getPlayerSymbol(int64_t player_id) const;
};