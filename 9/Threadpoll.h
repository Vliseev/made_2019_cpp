//
// Created by user on 10.12.2019.
//

#pragma once
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>

/*!
 * @brief  C++ Concurrency in Action by Anthony Williams ch9 example 2
 */
class FunctionWrapper {
    struct ImplBase {
        virtual void Call() = 0;
        virtual ~ImplBase() = default;
    };
    std::unique_ptr<ImplBase> impl_;

    template <typename F>
    struct ImplType : ImplBase {
        F f_;
        explicit ImplType(F &&f_) : f_(std::move(f_)) {}
        void Call() override {
            f_();
        }
    };

 public:
    FunctionWrapper() : impl_(nullptr) {}

    template <typename F>
    explicit FunctionWrapper(F &&f) : impl_(new ImplType<F>(std::move(f))) {}

    void Call() {
        impl_->Call();
    }

    FunctionWrapper(FunctionWrapper &&other) noexcept
        : impl_(std::move(other.impl_)) {}

    FunctionWrapper &operator=(FunctionWrapper &&other) noexcept {
        impl_ = std::move(other.impl_);
        return *this;
    }

    FunctionWrapper(const FunctionWrapper &) = delete;
    FunctionWrapper(FunctionWrapper &) = delete;
    FunctionWrapper &operator=(const FunctionWrapper &) = delete;
};

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

    bool can_stop_;
    std::vector<std::thread> workers_;
    std::queue<FunctionWrapper> task_queue_;

    void WorkerFunc();
};

ThreadPool::ThreadPool(size_t size) : can_stop_(false) {
    for (size_t i = 0; i < size; ++i) {
        workers_.emplace_back(&ThreadPool::WorkerFunc, this);
    }
}

void ThreadPool::WorkerFunc() {
    FunctionWrapper task;
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
        task.Call();
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lk(mut_);
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

    task_queue_.emplace(std::move(task));
    con_.notify_one();

    return future;
}

template <typename Func>
auto ThreadPool::Exec(Func &&f) {
    using return_type = decltype(f());

    std::packaged_task<return_type()> task(std::forward<Func>(f));
    std::future<return_type> future = task.get_future();

    task_queue_.emplace(std::move(task));
    con_.notify_one();

    return future;
}
