#pragma once

#include <tgbot/tgbot.h>

#include <memory>

#include "task_manager/task_manager.h"

class Command
{
   public:
    Command(TgBot::Bot* bot, std::shared_ptr<TaskManager> tm) : bot_(bot), tm_(tm) {}

    virtual ~Command() = default;
    virtual std::string command() const = 0;
    virtual std::string description() const = 0;
    virtual void execute(const TgBot::Message::Ptr& message) = 0;

   protected:
    TgBot::Bot* bot_;
    std::shared_ptr<TaskManager> tm_;
};