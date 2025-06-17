#pragma once

#include <chrono>

#include "common.h"

class DriveChecker4
{
public:

    auto GetAllDriveSpace() -> std::vector<DriveInfo>;

private:
    bool CheckTimeout(const std::string& path, std::chrono::milliseconds timeout);
};