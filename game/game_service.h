#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "db/db_service.h"
#include "game_logic.h"

class GameService
{
   public:
    explicit GameService(std::shared_ptr<DbService> db);

    int createGame(int64_t player1_id, int64_t player2_id);
    std::optional<std::shared_ptr<GameLogic>> getGame(int game_id);

    std::optional<std::shared_ptr<GameLogic>> getUserActiveGame(int64_t tgId);

    bool makeMove(int game_id, int64_t player_id, int column);
    bool isGameOver(int game_id, int64_t& winner_id);
    std::string renderGameBoard(int game_id);

    bool isPlayerTurn(int game_id, int64_t player_id);
    std::optional<int64_t> getOpponent(int game_id, int64_t player_id);

    bool abandonGame(int game_id, int64_t player_id);

   private:
    std::shared_ptr<DbService> db_;

    std::unordered_map<int, std::shared_ptr<GameLogic>> gamesById_;
    std::unordered_map<int64_t, std::shared_ptr<GameLogic>> gamesByUser_;

    std::shared_ptr<GameLogic> getActiveGameById(int game_id);
    std::shared_ptr<GameLogic> getActiveGameByUser(int64_t user_id);
    void cacheGame(const std::shared_ptr<GameLogic>& game, int64_t player1_id, int64_t player2_id);
    void removeGameFromCache(int game_id, int64_t player1_id, int64_t player2_id);
};