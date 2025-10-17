#pragma once

#include <string>

#include "command.h"

class SendMessageCommand : public Command
{
   public:
    SendMessageCommand(TgBot::Bot* bot, std::shared_ptr<TaskManager> tm) : Command(bot, tm) {}
    std::string command() const override { return com; }
    std::string description() const override { return desc; }
    void execute(const TgBot::Message::Ptr& message) override
    {
        if (!message || !message->chat)
        {
            return;
        }

        std::istringstream iss(message->text);
        std::string command;
        iss >> command;
        std::string mess;
        std::getline(iss, mess);

        if (!mess.empty() && mess[0] == ' ')
        {
            mess.erase(0, 1);
        }

        if (iss.fail())
        {
            bot_->getApi().sendMessage(message->chat->id, "[Invalid format. Use: /message <text>]");
            return;
        }

        SendMessageParams params;
        params.from_id = message->chat->id;
        params.message = mess;
        params.callback = [this, chatId = message->chat->id](bool succ, std::string message, int64_t target)
        {
            if (succ)
            {
                bot_->getApi().sendMessage(target, message);
                bot_->getApi().sendMessage(chatId, "[✅Message delivered]");
            }
            else
            {
                bot_->getApi().sendMessage(chatId, "[⚠️ Message not delivered]");
                bot_->getApi().sendMessage(chatId, message);
            }
        };

        tm_->addSendMessageTask(params);
    }

   private:
    const std::string desc{"Send a message to your opponent."};
    const std::string com{"/send_message"};
};