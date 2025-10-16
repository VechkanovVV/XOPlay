#pragma once

#include <sstream>

#include "db_service.h"
#include "task.h"

class GetHistoryTask final : public Task
{
   public:
    GetHistoryTask(std::shared_ptr<DbService> db, GetHistoryParams params) : db_(db), params_(std::move(params)) {}
    void execute() override
    {
        auto history = db_->getHistoryByPlayer(params_.player_id);
        std::stringstream ss;
        for (auto& i : history)
        {
            auto game = db_->getGameById(i.game_id);
            if (game)
            {
                ss << "[Opponent TG ID: "
                   << (game.value().first_player_tg == params_.player_id ? game.value().second_player_tg
                                                                         : game.value().first_player_tg);
                ss << ", Result: " << i.result << "]\n";
            }
        }

        std::string result = ss.str();
        params_.callback(result);
    }

   private:
    std::shared_ptr<DbService> db_;
    GetHistoryParams params_;
};