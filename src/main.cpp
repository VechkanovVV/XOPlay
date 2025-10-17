#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>

#include "bot.h"
#include "config.h"
#include "db.h"
#include "db_service.h"
#include "game/game_service.h"
#include "task_manager/task_manager.h"
#include "thread"
#include "thread_pool.h"

namespace
{
constexpr int portdb = 5432;
std::atomic<bool> shutdownRequested{false};
}  // namespace

void signalHandler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        std::cout << "\nShutdown signal received. Stopping bot..." << std::endl;
        shutdownRequested = true;
    }
}

int main()
{
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    try
    {
        auto config = std::make_shared<Config>();

        std::string dbConnStr =
            "host=db port=" + std::to_string(portdb) + " dbname=" + config->get(ConfigVariable::DbName) +
            " user=" + config->get(ConfigVariable::DbUser) + " password=" + config->get(ConfigVariable::DbPassword);

        auto db = std::make_shared<Database>(dbConnStr);
        auto dbService = std::make_shared<DbService>(db);
        auto gameService = std::make_shared<GameService>(dbService);
        auto tp = std::make_shared<ThreadPool>(std::thread::hardware_concurrency());
        auto taskManager = std::make_shared<TaskManager>(dbService, gameService, tp);
        taskManager->start();

        std::string token = config->get(ConfigVariable::BotToken);
        Bot bot(token, taskManager);

        std::cout << "Bot started. Press Ctrl+C to exit." << std::endl;

        std::thread botThread([&bot]() { bot.run(); });

        while (!shutdownRequested)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Stopping task manager..." << std::endl;
        taskManager->stop();

        std::cout << "Waiting for bot thread to finish..." << std::endl;
        if (botThread.joinable())
        {
            botThread.join();
        }

        std::cout << "Bot stopped successfully." << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}