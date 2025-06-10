#include "DriveChecker2.h"

#include <iostream>
#include "windows.h"
#include "winnetwk.h"
#include "shlwapi.h"

DriveChecker2::DriveChecker2()
{
}

DriveChecker2::~DriveChecker2()
{
}

auto DriveChecker2::GetAllDriveSpace() -> std::vector<DriveInfo>
{
    std::vector<DriveInfo> driveInfos;
    auto drives = GetDrives();
    for (const auto& drive : drives)
    {
        std::cout << "Start get info " << drive << std::endl;
        auto driveType = GetDriveTypeA(drive.c_str());
        if (driveType == DRIVE_NO_ROOT_DIR || driveType == DRIVE_UNKNOWN)
        {
            std::cout << drive << " is invalid" << std::endl;
            continue; // Skip invalid drives
        }

        //ネットワークドライブの場合、接続しているか確認する
        if (driveType == DRIVE_REMOTE)
        {
            std::cout << "Check network drive is connected " << drive << std::endl;

            // イーサネットを無効化してネットワークから切断してもNO_ERRORだった。
            // 接続の有無とは関係無いらしい
            //auto driveLetter = drive.substr(0, 2); // Get the drive letter (e.g., "Z:")
            //DWORD dwResult;
            //CHAR szRemoteName[MAX_PATH] = { 0 };
            //DWORD dwSize = MAX_PATH;
            //dwResult = WNetGetConnectionA(driveLetter.c_str(), szRemoteName, &dwSize);
            //if (dwResult != NO_ERROR)
            //{
            //    std::cout << driveLetter << " is not connected " << dwResult << std::endl;
            //    continue; // Skip disconnected network drives
            //}
            
            // ネットワークドライブが接続しているかは無関係で、パスのみで判定する
            auto isNetworkPath = PathIsNetworkPathA(drive.c_str());
            std::cout << "Is network path: " << (isNetworkPath ? "Yes" : "No") << std::endl;

            DWORD attr = GetFileAttributesA(drive.c_str());
            if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_REPARSE_POINT) == 0)
            {
                // ネットワークドライブが接続されていてもここに入る。使えない。
                std::cout << drive << " is not connected !!!!" << std::endl;
                continue; // Skip disconnected network drives
            }
        }

        // ネットワークが接続していない時、ネットワークドライブに対しての
        // GetDiskFreeSpaceExAにかかる時間が不定。一瞬で終わる事もあれば10上以上待つこともある
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
        if (GetDiskFreeSpaceExA(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            DriveInfo info;
            info.name = drive;
            info.type = GetDriveTypeStr(driveType);;
            info.totalSpace = totalNumberOfBytes.QuadPart;
            info.freeSpace = totalNumberOfFreeBytes.QuadPart;
            driveInfos.push_back(info);

            PrintInfo(info);
        }
    }
    return driveInfos;
}