#include "common.h"

#include <iostream>
#include <windows.h>

auto GetDrives() -> std::vector<std::string>
{
    constexpr unsigned int bufferSize = 256;
    char buffer[bufferSize];
    const auto result = GetLogicalDriveStringsA(bufferSize, buffer);

    if (0 < result && result < bufferSize)
    {
        std::vector<std::string> drives;
        char* drive = buffer;
        while (*drive)
        {
            drives.emplace_back(drive);
            drive += strlen(drive) + 1; // Move to the next drive string
        }
        return drives;
    }
    else
    {
        return {}; // Return an empty vector if no drives found or error occurred
    }
}

auto GetDriveTypeStr(unsigned int driveType) -> std::string
{
    switch (driveType)
    {
        case DRIVE_UNKNOWN: return "Unknown";
        case DRIVE_NO_ROOT_DIR: return "No Root Directory";
        case DRIVE_REMOVABLE: return "Removable";
        case DRIVE_FIXED: return "Fixed";
        case DRIVE_REMOTE: return "Remote";
        case DRIVE_CDROM: return "CD-ROM";
        case DRIVE_RAMDISK: return "RAM Disk";
        default: return "Invalid Drive Type";
    }
}

void PrintInfo(const DriveInfo& info)
{
    std::cout << "Drive: " << info.name << "\n"
        << "Type: " << info.type << "\n"
        << "Total Space: " << info.totalSpace / (1024 * 1024 * 1024) << " GB\n"
        << "Free Space: " << info.freeSpace / (1024 * 1024 * 1024) << " GB\n"
        << std::endl;
}