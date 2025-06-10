#pragma once

#include <unordered_map>
#include <future>
#include <filesystem>
#include <memory>
#include <optional>

#include "common.h"

class DriveChecker3 {
public:
    ~DriveChecker3();

    auto GetAllDriveSpace() -> std::vector<DriveInfo>;

private:
    bool checkAccessible(const std::string& path, std::chrono::milliseconds timeout);

    struct DriveState {
        std::optional<std::future<bool>> future;
    };

    std::unordered_map<std::string, DriveState> states_;
    std::mutex mutex_;
};

