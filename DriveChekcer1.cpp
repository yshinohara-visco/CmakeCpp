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
        auto driveType = GetDriveTypeA(drive.c_str());
        if (driveType == DRIVE_NO_ROOT_DIR || driveType == DRIVE_UNKNOWN)
        {
            std::cout << drive << " is invalid" << std::endl;
            continue; // Skip invalid drives
        }

        // �l�b�g���[�N���ڑ����Ă��Ȃ����A�l�b�g���[�N�h���C�u�ɑ΂��Ă�
        // GetDiskFreeSpaceExA�ɂ����鎞�Ԃ��s��B��u�ŏI��鎖�������10��ȏ�҂��Ƃ�����
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