#pragma once

#include <string>
#include <chrono>

class StopWatch
{
public:
    StopWatch();

    void Start(const std::string& name);
    void Wrap(const std::string& message);
    void Print();

private:
    std::string name_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::vector<std::pair<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>>> wraps_;
};

class StopWatchG
{
public:
    static void Start(const std::string& name);
    static void Wrap(const std::string& message);
    static void Print();

private:
    static StopWatch sw_;
};