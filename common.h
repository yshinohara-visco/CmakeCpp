#pragma once

#include <vector>
#include <string>

struct DriveInfo
{
    std::string name;
    std::string type;
    unsigned long long totalSpace;
    unsigned long long freeSpace;
};

auto GetDrives() -> std::vector<std::string>;

auto GetDriveTypeStr(unsigned int driveType) -> std::string;

void PrintInfo(const DriveInfo& info);