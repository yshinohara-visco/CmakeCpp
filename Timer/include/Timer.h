#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace TimerLib {

class Timer {
public:
    Timer(const std::string& name = "Timer");
    ~Timer();

    void Start();
    void Wrap(const std::string& message = "");
    void Stop();
    void Reset();

    void SetDelayedOutput(bool delayed);
    void FlushOutput();

    std::string GetFormattedResult() const;

private:
    struct TimePoint {
        std::chrono::high_resolution_clock::time_point time;
        std::string message;
        bool is_wrap;
    };

    std::string name_;
    std::chrono::high_resolution_clock::time_point start_time_;
    std::vector<TimePoint> time_points_;
    bool is_running_;
    bool delayed_output_;
    mutable std::mutex mutex_;

    std::string FormatDuration(const std::chrono::nanoseconds& duration) const;
    std::string GetFormattedResultInternal() const;
};

class TimerManager {
public:
    static TimerManager& Instance();
    
    void RegisterTimer(std::shared_ptr<Timer> timer);
    void FlushAllTimers();
    void SetGlobalDelayedOutput(bool delayed);

private:
    TimerManager() = default;
    std::vector<std::weak_ptr<Timer>> timers_;
    std::mutex mutex_;
    bool global_delayed_output_ = false;
};

}