#pragma once

#include "command.h"

class GetHistoryCommand : public Command
{
   public:
    GetHistoryCommand(TgBot::Bot* bot, std::shared_ptr<TaskManager> tm) : Command(bot, tm) {}
    std::string command() const override { return "/history"; }
    std::string description() const override { return "Get your game history."; }

    void execute(const TgBot::Message::Ptr& message) override
    {
        if (!message || !message->from)
        {
            return;
        }

        GetHistoryParams params;
        params.player_id = message->chat->id;
        params.callback = [this, chatId = message->chat->id](const std::string& result)
        { bot_->getApi().sendMessage(chatId, result); };

        tm_->addGetHistoryTask(params);
    }
};