#pragma once

#include <functional>
#include <thread>
#include <vector>

#include "threadsafe_queue.h"

class ThreadPool final
{
   public:
    explicit ThreadPool(size_t num);
    ~ThreadPool();

    void addTask(std::function<void()> task);
    void stop_and_wait();

   private:
    size_t threadsCount_;
    ThreadsafeQueue<std::function<void()>> tasks_;
    std::vector<std::thread> workers_;
};
