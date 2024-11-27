#pragma once

#include <iostream>
#include <coroutine>

struct Task
{
    struct promise_type
    {
        Task get_return_object()
        {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
    
        // 最初に呼ばれる
        auto initial_suspend()
        {
            std::cout << "initial_suspend" << std::endl;
            return std::suspend_always{}; // 止まった状態でスタート
        }
    
        // 最後に呼ばれる
        auto final_suspend() noexcept
        {
            std::cout << "final_suspend" << std::endl;
            return std::suspend_always{};
        }
    
        void return_void() {}
    
        void unhandled_exception()
        {
            std::terminate();
        }
    };

    std::coroutine_handle<promise_type> handle;
    std::mutex mtx;

    Task(std::coroutine_handle<promise_type> h) : handle(h) {}

    ~Task()
    {
        std::lock_guard<std::mutex> lock(mtx);
        if(handle && !handle.done())
            handle.destroy();
    }

    void resume()
    {
        std::lock_guard<std::mutex> lock(mtx);
        if(handle && !handle.done())
            handle.resume();
    }

    void abort()
    {
        std::lock_guard<std::mutex> lock(mtx);
        if(handle && !handle.done())
        {
            handle.destroy();
            handle = nullptr;
        }
    }

    bool is_suspend()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return handle && !handle.done();
    }
};

Task DoTasks();

Task LongTasks();

