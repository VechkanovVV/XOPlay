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

    bool makeMove(int game_id, int64_t player_id, int column);
    bool isGameOver(int game_id, int64_t& winner_id);
    std::string renderGameBoard(int game_id);

    bool isPlayerTurn(int game_id, int64_t player_id);
    std::optional<int64_t> getOpponent(int game_id, int64_t player_id);

    bool abandonGame(int game_id, int64_t player_id);

   private:
    std::shared_ptr<DbService> db_;

    std::unordered_map<int, std::shared_ptr<GameLogic>> active_games_cache_;

    std::shared_ptr<GameLogic> getOrCreateGameInCache(int game_id);
};