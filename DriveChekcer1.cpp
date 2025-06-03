#include "DriveChekcer1.h"

#include <iostream>
#include "Windows.h"

DriveChecker1::DriveChecker1()
{
}

DriveChecker1::~DriveChecker1()
{
}

auto DriveChecker1::GetAllDriveSpace() -> std::vector<DriveInfo>
{
    std::vector<DriveInfo> driveInfos;
    auto drives = GetDrives();
    for (const auto& drive : drives)
    {
        std::cout << "Start get info " << drive << std::endl;
        auto drive_type = GetDriveTypeA(drive.c_str());
        if (drive_type == DRIVE_NO_ROOT_DIR || drive_type == DRIVE_UNKNOWN)
        {
            std::cout << drive << " is invalid" << std::endl;
            continue; // Skip invalid drives
        }

        // ネットワークが接続していない時、ネットワークドライブに対しての
        // GetDiskFreeSpaceExAにかかる時間が不定。一瞬で終わる事もあれば10上以上待つこともある
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
        if (GetDiskFreeSpaceExA(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            DriveInfo info;
            info.name = drive;
            info.type = GetDriveTypeStr(drive_type);;
            info.totalSpace = totalNumberOfBytes.QuadPart;
            info.freeSpace = totalNumberOfFreeBytes.QuadPart;
            driveInfos.push_back(info);

            PrintInfo(info);
        }
    }
    return driveInfos;
}