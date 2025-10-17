#pragma once

#include "command.h"

class HelpCommand : public Command
{
   public:
    HelpCommand(TgBot::Bot* bot, std::shared_ptr<TaskManager> tm) : Command(bot, tm) {}
    std::string command() const override { return com; }
    std::string description() const override { return desc; }
    void execute(const TgBot::Message::Ptr& message) override
    {
        if (!message || !message->from)
        {
            return;
        }

        HelpParams params;
        params.player_id = message->chat->id;
        params.callback = [this, chatId = message->chat->id](std::string result)
        { bot_->getApi().sendMessage(chatId, result); };

        tm_->addHelpTask(params);
    }

   private:
    const std::string desc{"Shows a list of all available commands."};
    const std::string com{"/help"};
};