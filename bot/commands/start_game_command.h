#pragma once

#include <string>

#include "command.h"

class StartGameCommand : public Command
{
   public:
    StartGameCommand(TgBot::Bot* bot, std::shared_ptr<TaskManager> tm) : Command(bot, tm) {}
    std::string command() const override { return com; }
    std::string description() const override { return desc; }
    void execute(const TgBot::Message::Ptr& message) override
    {
        StartGameParams params;
        params.player_id = message->chat->id;
        params.callback = [this, chatId = message->chat->id](std::string message)
        { bot_->getApi().sendMessage(chatId, message); };

        tm_->addStartGameTask(params);
    }

   private:
    const std::string desc{"Starts a game and finds an opponent."};
    const std::string com{"/start_game"};
};