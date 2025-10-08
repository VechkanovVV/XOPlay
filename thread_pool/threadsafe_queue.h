#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <utility>

template <typename T>
class ThreadsafeQueue final
{
   public:
    ThreadsafeQueue() = default;
    ~ThreadsafeQueue() = default;
    ThreadsafeQueue(const ThreadsafeQueue&) = delete;
    ThreadsafeQueue& operator=(const ThreadsafeQueue&) = delete;

    void push(T value)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (stopped_.load())
        {
            return;
        }

        dataQueue_.push(std::move(value));
        datacv_.notify_one();
    }

    void stop()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        stopped_.store(true);
        datacv_.notify_all();
    }

    bool waitAndPop(T& value)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        datacv_.wait(lock, [this] { return !dataQueue_.empty() || stopped_.load(); });

        if (stopped_.load() && dataQueue_.empty())
        {
            return false;
        }

        value = std::move(dataQueue_.front());
        dataQueue_.pop();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return dataQueue_.empty();
    }

   private:
    mutable std::mutex mtx_;
    std::queue<T> dataQueue_;
    std::condition_variable datacv_;
    std::atomic<bool> stopped_{false};
};