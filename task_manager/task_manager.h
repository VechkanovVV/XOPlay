#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <queue>

#include "db_service.h"
#include "game/game_service.h"
#include "task/task.h"
#include "task/task_params.h"
#include "thread_pool.h"
#include "threadsafe_queue.h"

class TaskManager final
{
   public:
    TaskManager(std::shared_ptr<DbService> db, std::shared_ptr<GameService> gs, std::shared_ptr<ThreadPool> threadPool);
    ~TaskManager();

    void start();

    void stop();

    void addStartGameTask(StartGameParams params);
    void addStopGameTask(StopGameParams params);
    void addMakeMoveTask(MakeMoveParams params);
    void addSendMessageTask(SendMessageParams params);
    void addGetHistoryTask(GetHistoryParams params);
    void addHelpTask(HelpParams params);

   private:
    std::shared_ptr<DbService> db_;
    std::shared_ptr<GameService> gs_;
    std::shared_ptr<ThreadPool> threadPool_;

    ThreadsafeQueue<std::shared_ptr<Task>> taskQueue_;
    ThreadsafeQueue<StartGameParams> waitToStartQueue_;

    std::atomic<bool> running_;

    std::thread worker_;
    std::mutex registrationMutex_;

    void addTask(std::shared_ptr<Task> task);
    void ensureUserRegistered(int64_t user_id, const std::string& username = "unknown",
                              const std::string& first_name = "unknown", const std::string& last_name = "unknown");
};