#include "bot.h"

#include <stdexcept>

#include "commands/get_history_command.h"
#include "commands/help_command.h"
#include "commands/make_move_command.h"
#include "commands/send_message_command.h"
#include "commands/start_game_command.h"
#include "commands/stop_game_command.h"

Bot::Bot(std::string& token, std::shared_ptr<TaskManager> taskManager) : token_(token), taskManager_(taskManager)
{
    if (token.empty() || !taskManager_)
    {
        throw std::invalid_argument("Bot dependencies must not be null");
    }
    bot_ = std::make_unique<TgBot::Bot>(token);

    registerCommands();
    registerDefaultHandlers();
}

void Bot::run()
{
    TgBot::TgLongPoll longPoll(*bot_);

    while (true)
    {
        longPoll.start();
    }
}

void Bot::registerCommands()
{
    commands_.emplace("/start", std::make_unique<StartGameCommand>(bot_.get(), taskManager_));
    commands_.emplace("/help", std::make_unique<HelpCommand>(bot_.get(), taskManager_));
    commands_.emplace("/move", std::make_unique<MakeMoveCommand>(bot_.get(), taskManager_));
    commands_.emplace("/stop", std::make_unique<StopGameCommand>(bot_.get(), taskManager_));
    commands_.emplace("/message", std::make_unique<SendMessageCommand>(bot_.get(), taskManager_));
    commands_.emplace("/history", std::make_unique<GetHistoryCommand>(bot_.get(), taskManager_));
}

void Bot::registerDefaultHandlers()
{
    for (const auto& [commandName, commandHandler] : commands_)
    {
        std::string cmdName = commandName.substr(1);

        bot_->getEvents().onCommand(cmdName,
                                    [this, commandName](TgBot::Message::Ptr message)
                                    {
                                        auto it = commands_.find(commandName);
                                        if (it != commands_.end())
                                        {
                                            it->second->execute(message);
                                        }
                                    });
    }
    bot_->getEvents().onNonCommandMessage(
        [this](TgBot::Message::Ptr message)
        {
            if (!message || message->text.empty())
            {
                return;
            }
            handleTextMessage(message);
        });
}

void Bot::handleCommand(const TgBot::Message::Ptr& message, const std::string& command)
{
    auto it = commands_.find(command);
    if (it == commands_.end())
    {
        bot_->getApi().sendMessage(message->chat->id, "Unknown command. Use /help.");
        return;
    }
    it->second->execute(message);
}

void Bot::handleTextMessage(const TgBot::Message::Ptr& message)
{
    auto command = commands_.find("/message");
    if (command == commands_.end())
    {
        bot_->getApi().sendMessage(message->chat->id, "Message sending is unavailable.");
        return;
    }
    command->second->execute(message);
}