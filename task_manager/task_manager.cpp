#include "task_manager.h"

#include <functional>

#include "task/get_history_task.h"
#include "task/help_task.h"
#include "task/make_move_task.h"
#include "task/send_message_task.h"
#include "task/start_game_task.h"
#include "task/stop_game_task.h"

TaskManager::TaskManager(std::shared_ptr<DbService> db, std::shared_ptr<GameService> gs,
                         std::shared_ptr<ThreadPool> threadPool)
    : db_(std::move(db)), gs_(std::move(gs)), threadPool_(std::move(threadPool)), running_(false)
{
}

TaskManager::~TaskManager()
{
    stop();
}

void TaskManager::start()
{
    if (running_.exchange(true)) return;
    worker_ = std::thread(
        [this]
        {
            while (running_)
            {
                std::shared_ptr<Task> task;
                if (!taskQueue_.waitAndPop(task)) return;
                if (!task) continue;
                threadPool_->addTask([t = std::move(task)] { t->execute(); });
            }
        });
}

void TaskManager::stop()
{
    if (!running_.exchange(false)) return;
    taskQueue_.stop();
    if (worker_.joinable()) worker_.join();
}

void TaskManager::addTask(std::shared_ptr<Task> task)
{
    if (!running_) return;
    taskQueue_.push(std::move(task));
}

void TaskManager::addStartGameTask(const StartGameParams& params)
{
    addTask(std::make_shared<StartGameTask>(gs_, params));
}

void TaskManager::addStopGameTask(const StopGameParams& params)
{
    addTask(std::make_shared<StopGameTask>(db_, params));
}

void TaskManager::addMakeMoveTask(const MakeMoveParams& params)
{
    addTask(std::make_shared<MakeMoveTask>(gs_, params));
}

void TaskManager::addSendMessageTask(const SendMessageParams& params)
{
    addTask(std::make_shared<SendMessageTask>(db_, params));
}

void TaskManager::addGetHistoryTask(const GetHistoryParams& params)
{
    addTask(std::make_shared<GetHistoryTask>(db_, params));
}

void TaskManager::addHelpTask(const HelpParams& params)
{
    addTask(std::make_shared<HelpTask>(params));
}