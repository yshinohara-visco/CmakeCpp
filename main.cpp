#include <iostream>
#include <thread>
#include <chrono>
#include <functional>

void measureTime(std::function<void()> func)
{
	auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = end - start;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    std::cout << "elapsed time: " << ms << "ms" << std::endl;
}


int main()
{
    // stdのスリープはWindowsでは思った以上に低い ->Linuxでは?
    auto sleepStd = [](std::chrono::milliseconds time) {
        std::this_thread::sleep_for(time);
    };

    // スリープの精度は高いがCPUを使い続ける
    auto sleepSpin = [](std::chrono::milliseconds time) {
        auto start = std::chrono::high_resolution_clock::now();
        while (std::chrono::high_resolution_clock::now() - start < time)
        {
            std::this_thread::yield(); //一応譲る
        }
    };

    // もっと良い方法は無いか？

    const auto time1 = std::chrono::milliseconds(1);
    const auto time2 = std::chrono::milliseconds(10);
    const auto time3 = std::chrono::milliseconds(100);

    auto doNtimes = [&](std::function<void()> func, uint32_t time) {
        for (uint32_t i = 0; i < time; i++)
        {
            func();
        }
    };  

    std::cout << "---------------------" << std::endl;
    doNtimes([&](){measureTime([&](){sleepStd(time1);});}, 10);
    std::cout << "-------" << std::endl;
    doNtimes([&](){measureTime([&](){sleepSpin(time1);});}, 10);

    std::cout << "---------------------" << std::endl;
    doNtimes([&](){measureTime([&](){sleepStd(time2);});}, 10);
    std::cout << "-------" << std::endl;
    doNtimes([&](){measureTime([&](){sleepSpin(time2);});}, 10);

    std::cout << "---------------------" << std::endl;
    doNtimes([&](){measureTime([&](){sleepStd(time3);});}, 10);
    std::cout << "-------" << std::endl;
    doNtimes([&](){measureTime([&](){sleepSpin(time3);});}, 10);
	return 0;
}

/*
結果

---------------------
elapsed time: 19ms
elapsed time: 4ms
elapsed time: 15ms
elapsed time: 1ms
elapsed time: 15ms
elapsed time: 16ms
elapsed time: 14ms
elapsed time: 14ms
elapsed time: 16ms
elapsed time: 7ms
-------
elapsed time: 1ms
elapsed time: 1ms
elapsed time: 1ms
elapsed time: 1ms
elapsed time: 1ms
elapsed time: 1ms
elapsed time: 1ms
elapsed time: 1ms
elapsed time: 1ms
elapsed time: 1ms
---------------------
elapsed time: 25ms
elapsed time: 14ms
elapsed time: 15ms
elapsed time: 15ms
elapsed time: 14ms
elapsed time: 22ms
elapsed time: 17ms
elapsed time: 14ms
elapsed time: 15ms
elapsed time: 16ms
-------
elapsed time: 10ms
elapsed time: 10ms
elapsed time: 10ms
elapsed time: 10ms
elapsed time: 10ms
elapsed time: 10ms
elapsed time: 10ms
elapsed time: 10ms
elapsed time: 10ms
elapsed time: 10ms
---------------------
elapsed time: 115ms
elapsed time: 110ms
elapsed time: 116ms
elapsed time: 102ms
elapsed time: 111ms
elapsed time: 101ms
elapsed time: 100ms
elapsed time: 105ms
elapsed time: 107ms
elapsed time: 112ms
-------
elapsed time: 100ms
elapsed time: 100ms
elapsed time: 100ms
elapsed time: 100ms
elapsed time: 100ms
elapsed time: 100ms
elapsed time: 100ms
elapsed time: 100ms
elapsed time: 100ms
elapsed time: 100ms
*/
