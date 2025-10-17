#pragma once

#include <memory>

#include "game/game_service.h"
#include "task.h"

class StopGameTask final : public Task
{
   public:
    StopGameTask(std::shared_ptr<GameService> gs, StopGameParams params) : gs_(gs), params_(std::move(params)) {}

    void execute() override
    {
        auto gameOpt = gs_->getUserActiveGame(params_.player_id);

        if (!gameOpt)
        {
            params_.callback("There is no active game", params_.player_id);
            return;
        }

        auto game = gameOpt.value();
        auto gameId = game->getId();
        auto opId = (game->getPlayer1() == params_.player_id) ? game->getPlayer2() : game->getPlayer1();

        bool success = gs_->abandonGame(gameId, params_.player_id);

        if (success)
        {
            params_.callback("Game finished, you lose", params_.player_id);
            params_.callback("Game finished, you win", opId);
        }
        else
        {
            params_.callback("Failed to abandon the game", params_.player_id);
        }
    }

   private:
    std::shared_ptr<GameService> gs_;
    StopGameParams params_;
};