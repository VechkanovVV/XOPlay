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
        auto resp = gs_->getUserActiveGame(params_.player_id);
        if (!resp)
        {
            if (params_.callback)
            {
                params_.callback(false, "There is no active game", nullptr);
            }
            return;
        }

        try
        {
            auto game = resp.value();
            auto game_id = game->getId();
            bool success = gs_->makeMove(game_id, params_.player_id, params_.position);

            if (success && params_.callback)
            {
                auto gameResult = gs_->getGame(game_id);
                if (gameResult)
                {
                    std::string board = gs_->renderGameBoard(game_id);
                    params_.callback(true, board, game);
                }
                else
                {
                    params_.callback(false, "Failed to get game state", game);
                }
            }
            else if (params_.callback)
            {
                params_.callback(false, "Invalid move", game);
            }
        }
        catch (std::exception& e)
        {
            if (params_.callback)
            {
                params_.callback(false, e.what(), resp.value());
            }
        }
    }

   private:
    std::shared_ptr<GameService> gs_;
    MakeMoveParams params_;
};