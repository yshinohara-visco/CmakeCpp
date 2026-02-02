#include "generator.h"
#include <iostream>
#include <thread>
#include <chrono>

// フィボナッチ数列を無限に生成
std::generator<uint64_t> fibonacci()
{
    uint64_t a = 1, b = 1;
    while (true)
    {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

// チェックポイント付き処理
std::generator<std::string> checkpointProcess()
{
    // 処理1
    std::cout << "処理1を実行中..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "処理1が完了" << std::endl;
    co_yield "checkpoint_1";

    // 処理2
    std::cout << "処理2を実行中..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "処理2が完了" << std::endl;
    co_yield "checkpoint_2";

    // 処理3
    std::cout << "処理3を実行中..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "処理3が完了" << std::endl;
    co_yield "checkpoint_3";

    std::cout << "全ての処理が完了" << std::endl;
}
