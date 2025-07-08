#include "Timer.h"
#include <algorithm>

namespace TimerLib {

Timer::Timer(const std::string& name)
    : name_(name)
    , is_running_(false)
    , delayed_output_(false)
{
}

Timer::~Timer() {
    if (is_running_) {
        Stop();
    }
    if (!delayed_output_) {
        FlushOutput();
    }
}

void Timer::Start() {
    std::lock_guard<std::mutex> lock(mutex_);
    start_time_ = std::chrono::high_resolution_clock::now();
    time_points_.clear();
    is_running_ = true;
}

void Timer::Wrap(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!is_running_) {
        return;
    }
    
    TimePoint tp;
    tp.time = std::chrono::high_resolution_clock::now();
    tp.message = message.empty() ? "Wrap " + std::to_string(time_points_.size() + 1) : message;
    tp.is_wrap = true;
    time_points_.push_back(tp);
}

void Timer::Stop() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!is_running_) {
        return;
    }
    
    TimePoint tp;
    tp.time = std::chrono::high_resolution_clock::now();
    tp.message = "Stop";
    tp.is_wrap = false;
    time_points_.push_back(tp);
    is_running_ = false;
    
    if (!delayed_output_) {
        FlushOutput();
    }
}

void Timer::Reset() {
    std::lock_guard<std::mutex> lock(mutex_);
    time_points_.clear();
    is_running_ = false;
}

void Timer::SetDelayedOutput(bool delayed) {
    std::lock_guard<std::mutex> lock(mutex_);
    delayed_output_ = delayed;
}

void Timer::FlushOutput() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (time_points_.empty()) {
        return;
    }
    
    std::cout << GetFormattedResult() << std::endl;
}

std::string Timer::GetFormattedResult() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (time_points_.empty()) {
        return "[" + name_ + "] No measurements recorded";
    }
    
    std::ostringstream oss;
    oss << "+-- Timer: " << name_ << " --+\n";
    
    auto previous_time = start_time_;
    auto total_duration = std::chrono::nanoseconds::zero();
    
    for (size_t i = 0; i < time_points_.size(); ++i) {
        const auto& tp = time_points_[i];
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time - previous_time);
        auto cumulative = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time - start_time_);
        
        oss << "| " << std::setw(12) << tp.message << ": " 
            << std::setw(10) << FormatDuration(duration) 
            << " (total: " << std::setw(10) << FormatDuration(cumulative) << ")\n";
        
        previous_time = tp.time;
        total_duration = cumulative;
    }
    
    oss << "+-- Total Time: " << FormatDuration(total_duration) << " --+";
    
    return oss.str();
}

std::string Timer::FormatDuration(const std::chrono::nanoseconds& duration) const {
    auto ns = duration.count();
    
    if (ns < 1000) {
        return std::to_string(ns) + " ns";
    } else if (ns < 1000000) {
        return std::to_string(ns / 1000) + "." + std::to_string((ns % 1000) / 100) + " μs";
    } else if (ns < 1000000000) {
        return std::to_string(ns / 1000000) + "." + std::to_string((ns % 1000000) / 100000) + " ms";
    } else {
        return std::to_string(ns / 1000000000) + "." + std::to_string((ns % 1000000000) / 100000000) + " s";
    }
}

// TimerManager implementation
TimerManager& TimerManager::Instance() {
    static TimerManager instance;
    return instance;
}

void TimerManager::RegisterTimer(std::shared_ptr<Timer> timer) {
    std::lock_guard<std::mutex> lock(mutex_);
    timers_.push_back(timer);
    
    // Clean up expired weak_ptr
    timers_.erase(
        std::remove_if(timers_.begin(), timers_.end(),
            [](const std::weak_ptr<Timer>& wp) { return wp.expired(); }),
        timers_.end()
    );
}

void TimerManager::FlushAllTimers() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& weak_timer : timers_) {
        if (auto timer = weak_timer.lock()) {
            timer->FlushOutput();
        }
    }
}

void TimerManager::SetGlobalDelayedOutput(bool delayed) {
    std::lock_guard<std::mutex> lock(mutex_);
    global_delayed_output_ = delayed;
    for (auto& weak_timer : timers_) {
        if (auto timer = weak_timer.lock()) {
            timer->SetDelayedOutput(delayed);
        }
    }
}

}