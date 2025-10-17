#pragma once

#include <string>

#include "command.h"

class StopGameCommand : public Command
{
   public:
    StopGameCommand(TgBot::Bot* bot, std::shared_ptr<TaskManager> tm) : Command(bot, tm) {}
    std::string command() const override { return com; }
    std::string description() const override { return desc; }
    void execute(const TgBot::Message::Ptr& message) override
    {
        StopGameParams params;
        params.player_id = message->chat->id;
        params.callback = [this](std::string message, int64_t target) { bot_->getApi().sendMessage(target, message); };

        tm_->addStopGameTask(params);
    }

   private:
    const std::string desc{"Stop the game — you lose."};
    const std::string com{"/stop_game"};
};