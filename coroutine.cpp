#include "coroutine.h"

// コルーチンで中断、再開ができる関数
Task DoTasks()
{
    // task1
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout <<  "Do task 1" << std::endl;
    co_await std::suspend_always{};

    // task2
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout <<  "Do task 2" << std::endl;
    co_await std::suspend_always{};

    // task3
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout <<  "Do task 3" << std::endl;
    co_return;
}

// 長い処理をするコルーチン
Task LongTasks()
{
    const int count = 1000;
    for(int i = 0; i < count; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Long task " << i << std::endl;
        co_await std::suspend_always{};
    }
    co_return;
}