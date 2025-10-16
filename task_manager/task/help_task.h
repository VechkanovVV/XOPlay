#pragma once

#include "task.h"

class HelpTask final : public Task
{
   public:
    explicit HelpTask(HelpParams params) : params_(std::move(params)){};
    void execute() override
    {
        std::string commands =
            "/start — start searching for an opponent\n"
            "/move <1-7> — make a move\n"
            "/message <text> — send a message to your opponent\n"
            "/stop — end the game\n"
            "/history — get game history\n"
            "/help — show help";

        params_.callback(commands);
    }

   private:
    HelpParams params_;
};