#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include <Windows.h>

void PreciseWait( std::chrono::milliseconds ms );

class PreciseWaiter
{
public:
    PreciseWaiter();
    ~PreciseWaiter();

    void PostTask( std::chrono::milliseconds ms, std::function< void() > task );
    void Abort();

private:
    void ThreadFunc( std::stop_token stop_token );

    std::jthread thread_;
    std::mutex mtx_;
    std::condition_variable cv_;

    std::chrono::time_point< std::chrono::steady_clock > end_;
    std::function< void() > task_;
    std::atomic_bool abort_;
};

void WaitableWait( std::chrono::milliseconds ms );

class Waitable
{
public:
    Waitable();
    ~Waitable();

    void PostTask( std::chrono::milliseconds ms, std::function< void() > task );
    void Abort();

private:
    void ThreadFunc(std::stop_token st);

    std::jthread thread_;
    HANDLE timer_;
    std::mutex mtx_;
    std::function< void() > task_;
    std::atomic_bool abort_;
};

void PeriodWait( std::chrono::milliseconds ms );