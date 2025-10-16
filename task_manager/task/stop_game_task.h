#pragma once

#include <memory>

#include "db_service.h"
#include "task.h"

class StopGameTask final : public Task
{
   public:
    StopGameTask(std::shared_ptr<DbService> db, StopGameParams params) : db_(db), params_(std::move(params)) {}
    void execute() override
    {
        auto game = db_->getGameById(params_.game_id);
        if (!game) params_.callback(false);
        db_->updateGameStatus(game.value().id, "finished");
        db_->addHistoryRecord(params_.game_id, params_.player_id, "loose");
        auto op = game.value().first_player_tg == params_.player_id ? game.value().second_player_tg
                                                                    : game.value().first_player_tg;
        db_->addHistoryRecord(params_.game_id, op, "win");
        params_.callback(true);
    }

   private:
    std::shared_ptr<DbService> db_;
    StopGameParams params_;
};