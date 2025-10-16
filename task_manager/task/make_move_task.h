#pragma once

#include <memory>

#include "game/game_service.h"
#include "game_logic.h"
#include "task.h"

class MakeMoveTask final : public Task
{
   public:
    MakeMoveTask(std::shared_ptr<GameService> gs, MakeMoveParams params) : gs_(gs), params_(std::move(params)) {}
    void execute() override
    {
        try
        {
            bool success = gs_->makeMove(params_.game_id, params_.player_id, params_.position);

            if (success && params_.callback)
            {
                auto gameResult = gs_->getGame(params_.game_id);
                if (gameResult)
                {
                    auto game = gameResult.value();
                    std::string board = gs_->renderGameBoard(params_.game_id);
                    params_.callback(true, board);
                }
                else
                {
                    params_.callback(false, "Failed to get game state");
                }
            }
            else if (params_.callback)
            {
                params_.callback(false, "Invalid move");
            }
        }

        catch (std::exception& e)
        {
            if (params_.callback)
            {
                params_.callback(false, e.what());
            }
        }
    }

   private:
    std::shared_ptr<GameService> gs_;
    MakeMoveParams params_;
};