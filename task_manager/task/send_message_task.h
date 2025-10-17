#pragma once

#include "db_service.h"
#include "task.h"

class SendMessageTask final : public Task
{
   public:
    SendMessageTask(std::shared_ptr<GameService> gs, SendMessageParams params) : gs_(gs), params_(std::move(params)) {}

    void execute() override
    {
        auto resp = gs_->getUserActiveGame(params_.from_id);
        if (!resp)
        {
            if (params_.callback)
            {
                params_.callback(false, "[There is no active game]", params_.from_id);
            }
            return;
        }

        try
        {
            auto game = resp.value();
            auto op = game->getPlayer1() == params_.from_id ? game->getPlayer2() : game->getPlayer1();
            params_.callback(true, "[From opponent] " + params_.message, op);
        }
        catch (std::exception& e)
        {
            if (params_.callback)
            {
                params_.callback(false, e.what(), params_.from_id);
            }
        }
    }

   private:
    std::shared_ptr<GameService> gs_;
    SendMessageParams params_;
};
