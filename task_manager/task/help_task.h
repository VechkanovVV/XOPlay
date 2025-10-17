#pragma once

#include <sstream>

#include "task.h"

class HelpTask final : public Task
{
   public:
    HelpTask(HelpParams params) : params_(std::move(params)) {}

    void execute() override
    {
        std::stringstream ss;
        ss << "[Available commands:\n"
           << "/start - Start a new game\n"
           << "/move <column> - Make a move\n"
           << "/stop - Stop current game\n"
           << "/history - Show your game history\n"
           << "/message - Send a message to opponent\n"
           << "/help - Show this help message]";

        params_.callback(ss.str());
    }

   private:
    HelpParams params_;
};