#include "DriveChecker4.h"

#include <iostream>
#include <filesystem>
#include <future>
#include "Windows.h"

auto DriveChecker4::GetAllDriveSpace() -> std::vector<DriveInfo>
{
    std::vector<DriveInfo> driveInfos;
    auto drives = GetDrives();
    std::mutex mutex_;

    auto get_info = [&](const std::string& drive) -> bool
    {
        auto driveType = GetDriveTypeA(drive.c_str());

        // ネットワークドライブはタイムアウト付きで確認
        if (driveType == DRIVE_REMOTE)
        {
            std::cout << "Checking network drive: " << drive << std::endl;
            if (!CheckTimeout(drive, std::chrono::milliseconds(100))) {
                std::cout << "Network drive: " << drive << " is not accessible (timed out or not connected)." << std::endl;
                return false; // Skip inaccessible network drives
            }
        }
        else if (driveType == DRIVE_NO_ROOT_DIR || driveType == DRIVE_UNKNOWN) {
            std::cout << drive << " is invalid" << std::endl;
            return false; // Skip invalid drives
        }


        if (!std::filesystem::exists(drive)) {
            std::cout << "Drive: " << drive << " is not accessible (timed out or not connected)." << std::endl;
            return false; // Skip inaccessible drives
        }
        try {
            auto spaceInfo = std::filesystem::space(drive);
            DriveInfo info;
            info.name = drive;
            info.type = GetDriveTypeStr(driveType);
            info.totalSpace = spaceInfo.capacity;
            info.freeSpace = spaceInfo.free;
            PrintInfo(info);

            std::lock_guard<std::mutex> lock(mutex_); // Lock to protect shared resource
            driveInfos.push_back(info);
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error accessing drive " << drive << ": " << e.what() << std::endl;
            return false; // Skip inaccessible drives
        }
        return true;
    };

    // 並列起動
    std::vector<std::future<bool>> futures;
    for (const auto& drive : drives) 
    {
        futures.push_back(std::async(std::launch::async, get_info, drive));
    }
    // 全てのスレッドが完了するのを待つ
    for (auto& future : futures)
    {
        future.get(); // Wait for all futures to complete
    }



    //for (const auto& drive : drives)
    //{
    //    auto driveType = GetDriveTypeA(drive.c_str());

    //    // ネットワークドライブはタイムアウト付きで確認
    //    if(driveType == DRIVE_REMOTE)
    //    {
    //        std::cout << "Checking network drive: " << drive << std::endl;
    //        if (!CheckTimeout(drive, std::chrono::milliseconds(100))) {
    //            std::cout << "Network drive: " << drive << " is not accessible (timed out or not connected)." << std::endl;
    //            continue; // Skip inaccessible network drives
    //        }
    //    }
    //    else if (driveType == DRIVE_NO_ROOT_DIR || driveType == DRIVE_UNKNOWN) {
    //        std::cout << drive << " is invalid" << std::endl;
    //        continue; // Skip invalid drives
    //    }


    //    if (!std::filesystem::exists(drive)) {
    //        std::cout << "Drive: " << drive << " is not accessible (timed out or not connected)." << std::endl;
    //        continue; // Skip inaccessible drives
    //    }
    //    try {
    //        auto spaceInfo = std::filesystem::space(drive);
    //        DriveInfo info;
    //        info.name = drive;
    //        info.type = GetDriveTypeStr(driveType);
    //        info.totalSpace = spaceInfo.capacity;
    //        info.freeSpace = spaceInfo.free;
    //        driveInfos.push_back(info);
    //        PrintInfo(info);
    //    }
    //    catch (const std::filesystem::filesystem_error& e) {
    //        std::cerr << "Error accessing drive " << drive << ": " << e.what() << std::endl;
    //        continue; // Skip inaccessible drives
    //    }
    //}


    return driveInfos;
}

bool DriveChecker4::CheckTimeout(const std::string& path, std::chrono::milliseconds timeout)
{
    std::string command = "cmd /c dir \"" + path + "\" > nul"; // 接続してなければ失敗するコマンド
    std::string cmdLine = command; // CreateProcess に渡すための編集可能なコピー

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = {};

    if (!CreateProcessA(
        nullptr,
        cmdLine.data(),
        nullptr, nullptr, FALSE,
        CREATE_NO_WINDOW,
        nullptr, nullptr,
        &si, &pi)) {
        return false;
    }

    DWORD timeoutMs = static_cast<DWORD>(timeout.count());
    DWORD waitResult = WaitForSingleObject(pi.hProcess, timeoutMs);
    if (waitResult == WAIT_TIMEOUT) {
        TerminateProcess(pi.hProcess, 1); // タイムアウト時に強制終了
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return false;
    }

    DWORD exitCode = 1;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return exitCode == 0;
}