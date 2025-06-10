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

        //�l�b�g���[�N�h���C�u�̏ꍇ�A�ڑ����Ă��邩�m�F����
        if (driveType == DRIVE_REMOTE)
        {
            std::cout << "Check network drive is connected " << drive << std::endl;

            // �C�[�T�l�b�g�𖳌������ăl�b�g���[�N����ؒf���Ă�NO_ERROR�������B
            // �ڑ��̗L���Ƃ͊֌W�����炵��
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
            
            // �l�b�g���[�N�h���C�u���ڑ����Ă��邩�͖��֌W�ŁA�p�X�݂̂Ŕ��肷��
            auto isNetworkPath = PathIsNetworkPathA(drive.c_str());
            std::cout << "Is network path: " << (isNetworkPath ? "Yes" : "No") << std::endl;

            DWORD attr = GetFileAttributesA(drive.c_str());
            if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_REPARSE_POINT) == 0)
            {
                // �l�b�g���[�N�h���C�u���ڑ�����Ă��Ă������ɓ���B�g���Ȃ��B
                std::cout << drive << " is not connected !!!!" << std::endl;
                continue; // Skip disconnected network drives
            }
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