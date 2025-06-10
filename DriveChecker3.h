#pragma once

#include <unordered_map>
#include <future>
#include <filesystem>
#include <memory>
#include <optional>
#include <condition_variable>

#include "common.h"

class DriveChecker3 : public std::enable_shared_from_this<DriveChecker3>
{
private:
    DriveChecker3() = default;
public:
    static auto Create() -> std::shared_ptr<DriveChecker3>
    {
        return std::shared_ptr<DriveChecker3>(new DriveChecker3());
    }
    auto GetAllDriveSpace() -> std::vector<DriveInfo>;

private:
    bool checkAccessible(const std::string& path, std::chrono::milliseconds timeout);

    std::unordered_map<std::string, bool> checking_;
    std::unordered_map<std::string, bool> connected_;
    std::condition_variable cv_;
    std::mutex mutex_;
};

