#include "thread_pool.h"

#include <functional>

ThreadPool::ThreadPool(size_t num) : threadsCount_(num)
{
    for (size_t i = 0; i < threadsCount_; ++i)
    {
        workers_.emplace_back(
            [this]
            {
                while (true)
                {
                    std::function<void()> task;
                    bool popped = tasks_.waitAndPop(task);
                    if (!popped)
                    {
                        break;
                    }

                    task();
                }
            });
    }
}

ThreadPool::~ThreadPool()
{
    stop_and_wait();
}

void ThreadPool::addTask(std::function<void()> task)
{
    tasks_.push(task);
}
void ThreadPool::stop_and_wait()
{
    tasks_.stop();

    for (auto& w : workers_)
    {
        if (w.joinable())
        {
            w.join();
        }
    }
}
