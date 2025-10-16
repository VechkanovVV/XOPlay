#pragma once

#include <memory>

#include "game/game_service.h"
#include "task.h"

class StartGameTask final : public Task
{
   public:
    StartGameTask(std::shared_ptr<GameService> gs, StartGameParams params) : gs_(gs), params_(std::move(params)) {}
    void execute() override
    {
        auto res = gs_->createGame(params_.initiator_id, params_.opponent_id);
        params_.callback(res);
    }

   private:
    std::shared_ptr<GameService> gs_;
    StartGameParams params_;
};