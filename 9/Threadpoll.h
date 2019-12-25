//
// Created by user on 10.12.2019.
//

#pragma once
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
   public:
    explicit ThreadPool(size_t size);
    ~ThreadPool();

    template <typename Func, typename... Args>
    auto Exec(Func &&f, Args &&... args);

    template <typename Func>
    auto Exec(Func &&f);

   private:
    std::mutex mut_{};
    std::condition_variable con_{};

    uint64_t can_stop_;
    std::vector<std::thread> workers_;
    std::queue<std::packaged_task<void()>> task_queue_;

    void WorkerFunc();
};

ThreadPool::ThreadPool(size_t size) : can_stop_(false) {
    for (size_t i = 0; i < size; ++i) {
        workers_.emplace_back(&ThreadPool::WorkerFunc, this);
    }
}

void ThreadPool::WorkerFunc() {
    std::packaged_task<void()> task;
    while (true) {
        {
            std::unique_lock<std::mutex> lk(mut_);
            con_.wait(lk,
                      [this]() { return can_stop_ || !task_queue_.empty(); });
            if (can_stop_ && task_queue_.empty())
                return;
            task = std::move(task_queue_.front());
            task_queue_.pop();
        }
        task();
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lk(mut_);
        can_stop_ = true;
    }
    con_.notify_all();

    for (auto &&w : workers_) {
        w.join();
    }
}

template <typename Func, typename... Args>
auto ThreadPool::Exec(Func &&f, Args &&... args) {
    using return_type = typename std::result_of<Func(Args...)>::type;

    std::packaged_task<return_type()> task(
        std::bind<std::forward<Func>(f), std::forward<Args>(args)...>);
    auto future = task.get_future();
    {
        std::lock_guard<std::mutex> lk(mut_);
        task_queue_.emplace(std::packaged_task<void()>(task));
    }
    con_.notify_one();

    return future;
}

template <typename Func>
auto ThreadPool::Exec(Func &&f) {
    using return_type = decltype(f());

    std::packaged_task<return_type()> task(std::forward<Func>(f));
    std::future<return_type> future = task.get_future();

    {
        std::lock_guard<std::mutex> lk(mut_);
        task_queue_.emplace(std::move(task));
    }
    con_.notify_one();

    return future;
}
