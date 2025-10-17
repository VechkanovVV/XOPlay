#pragma once

#include <memory>

#include "game/game_service.h"
#include "task.h"

class StartGameTask final : public Task
{
   public:
    StartGameTask(std::shared_ptr<GameService> gs, StartGameParams params1, StartGameParams params2)
        : gs_(gs), params1_(std::move(params1)), params2_(std::move(params2))
    {
    }
    void execute() override
    {
        auto gameId = gs_->createGame(params1_.player_id, params2_.player_id);
        auto g = gs_->getGame(gameId);
        if (!g)
        {
            params1_.callback("Game does not exist");
            params2_.callback("Game does not exist");
        }

        auto game = g.value();
        auto firstId = game->getCurrentPlayer();
        StartGameParams& f = (firstId == params1_.player_id) ? params1_ : params2_;
        StartGameParams& s = (firstId != params1_.player_id) ? params1_ : params2_;

        f.callback("Game created, your move");
        s.callback("Game created, waiting for opponent’s move");
    }

   private:
    std::shared_ptr<GameService> gs_;
    StartGameParams params1_;
    StartGameParams params2_;
};