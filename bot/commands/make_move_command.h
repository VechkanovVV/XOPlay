#pragma once

#include <sstream>
#include <string>

#include "command.h"
#include "game/game_logic.h"
#include "task_manager/task/make_move_task.h"

class MakeMoveCommand : public Command
{
   public:
    MakeMoveCommand(TgBot::Bot* bot, std::shared_ptr<TaskManager> tm) : Command(bot, tm) {}
    std::string command() const override { return com; }
    std::string description() const override { return desc; }
    void execute(const TgBot::Message::Ptr& message) override
    {
        std::istringstream iss(message->text);
        std::string command;
        int pos;

        iss >> command >> pos;
        if (iss.fail())
        {
            bot_->getApi().sendMessage(message->chat->id, "Invalid format. Use: /move <column>");
            return;
        }

        MakeMoveParams params;
        params.player_id = message->chat->id;
        params.position = pos;
        params.callback =
            [this, chatId = message->chat->id](bool succ, std::string result, std::shared_ptr<GameLogic> gl)
        {
            if (succ && gl)
            {
                auto op = gl->getPlayer1() == chatId ? gl->getPlayer2() : gl->getPlayer1();
                bot_->getApi().sendMessage(op, gl->getBoardString());
                bot_->getApi().sendMessage(chatId, gl->getBoardString());
                bot_->getApi().sendMessage(chatId, result);
            }
            else
            {
                bot_->getApi().sendMessage(chatId, result);
            }
        };

        tm_->addMakeMoveTask(params);
    }

   private:
    const std::string desc{"Make a move on the game field."};
    const std::string com{"/move"};
};