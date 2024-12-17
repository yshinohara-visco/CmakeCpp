#include "stop_watch.h"

#include <iostream>

StopWatch::StopWatch()
{
    Start("");
}

void StopWatch::Start(const std::string& name)
{
    start_ = std::chrono::high_resolution_clock::now();
    name_ = name;
    wraps_.clear();
}

void StopWatch::Wrap(const std::string& message)
{
    wraps_.emplace_back(message, std::chrono::high_resolution_clock::now());
}

void StopWatch::Print()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);

    //std::cout << name_ << ": " << duration.count() << "us" << std::endl;
    printf("  %-10s: %5lld us\n", name_.c_str(), duration.count());

    for (const auto& wrap : wraps_)
    {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(wrap.second - start_);
        //std::cout << wrap.first << ": " << duration.count() << "us" << std::endl;
        printf("  %-12s: %5lld us\n", wrap.first.c_str(), duration.count());
    }
}

StopWatch StopWatchG::sw_ = StopWatch();
void StopWatchG::Start(const std::string& name)
{
    sw_.Start( name );
}
void StopWatchG::Wrap(const std::string& message)
{
    sw_.Wrap(message);
}
void StopWatchG::Print()
{
    sw_.Print();
}