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

void TaskManager::addStartGameTask(StartGameParams& params)
{
    if (gs_->getUserActiveGame(params.player_id))
    {
        params.callback("Finish the current game before starting a new one!");
        return;
    }
    if (waitToStartQueue_.empty())
    {
        if (params.callback)
        {
            params.callback("Waiting for an opponent...");
        }
        waitToStartQueue_.push(params);
    }
    else
    {
        StartGameParams op;
        bool suc = waitToStartQueue_.waitAndPop(op);
        if (!suc)
        {
            if (params.callback)
            {
                params.callback("Failed to find an opponent");
            }
            return;
        }

        if (params.player_id == op.player_id)
        {
            waitToStartQueue_.push(op);
            if (params.callback)
            {
                params.callback("Cannot play against yourself");
            }
            return;
        }

        addTask(std::make_shared<StartGameTask>(gs_, params, op));
    }
}

void TaskManager::addStopGameTask(const StopGameParams& params)
{
    addTask(std::make_shared<StopGameTask>(gs_, params));
}

void TaskManager::addMakeMoveTask(const MakeMoveParams& params)
{
    addTask(std::make_shared<MakeMoveTask>(gs_, params));
}

void TaskManager::addSendMessageTask(const SendMessageParams& params)
{
    addTask(std::make_shared<SendMessageTask>(gs_, params));
}

void TaskManager::addGetHistoryTask(const GetHistoryParams& params)
{
    addTask(std::make_shared<GetHistoryTask>(db_, params));
}

void TaskManager::addHelpTask(const HelpParams& params)
{
    addTask(std::make_shared<HelpTask>(params));
}