#pragma once

#include "db_service.h"
#include "task.h"

class SendMessageTask final : public Task
{
   public:
    SendMessageTask(std::shared_ptr<DbService> db, SendMessageParams params) : db_(db), params_(std::move(params)) {}

    void execute() override
    {
        auto game = db_->getActiveGameByPlayers(params_.from_id, params_.to_id);
        if (game && (game->status == "pending" || game->status == "active"))
        {
            params_.callback(true, params_.message);
        }
        else
        {
            params_.callback(false, "");
        }
    }

   private:
    std::shared_ptr<DbService> db_;
    SendMessageParams params_;
};
