#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "db.h"

struct User
{
    int64_t tg_id;
    std::string username;
    std::string first_name;
    std::string last_name;
};

struct Game
{
    int id;
    int64_t first_player_tg;
    int64_t second_player_tg;
    std::string status;
    std::string board;
};

struct HistoryRecord
{
    int id;
    int game_id;
    int64_t player_tg;
    std::string result;
};

class DbService
{
   public:
    explicit DbService(std::shared_ptr<Database> db);

    bool addUser(int64_t tg_id, const std::string& username, const std::string& first_name,
                 const std::string& last_name);
    std::optional<User> getUserById(int64_t id);

    int createGame(int64_t first_player_tg, int64_t second_player_tg);
    bool updateGameStatus(int game_id, const std::string& status);
    std::vector<Game> getGamesByPlayer(int64_t tg_id);
    std::optional<Game> getGameById(int64_t game_id);
    std::optional<Game> getActiveGameByPlayers(int64_t first_player_tg, int64_t second_player_tg);

    void addHistoryRecord(int game_id, int64_t player_tg, const std::string& result);
    std::vector<HistoryRecord> getHistoryByGame(int game_id);
    std::vector<HistoryRecord> getHistoryByPlayer(int64_t player_tg);

    bool updateGameBoard(int game_id, const std::string& board);
    void addMoveRecord(int game_id, int64_t player_tg, int column);

   private:
    std::shared_ptr<Database> db_;
};