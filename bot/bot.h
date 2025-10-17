#pragma once

#include <tgbot/tgbot.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "commands/command.h"
#include "task_manager/task_manager.h"

class Bot
{
   public:
    Bot(std::string& token, std::shared_ptr<TaskManager> taskManager);

    void run();

   private:
    void registerCommands();
    void registerDefaultHandlers();
    void handleCommand(const TgBot::Message::Ptr& message, const std::string& command);
    void handleTextMessage(const TgBot::Message::Ptr& message);

    std::string token_;
    std::shared_ptr<TaskManager> taskManager_;
    std::unique_ptr<TgBot::Bot> bot_;
    std::unordered_map<std::string, std::unique_ptr<Command>> commands_;
};