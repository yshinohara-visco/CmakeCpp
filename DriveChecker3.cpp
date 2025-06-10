#include "DriveChecker3.h"

#include <iostream>
#include "windows.h"

DriveChecker3::~DriveChecker3()
{
    std::unique_lock lock(mutex_);
    for (auto& [_, state] : states_) {
        if (state.future.has_value()) {
            state.future->wait();
        }
    }
}

auto DriveChecker3::GetAllDriveSpace() -> std::vector<DriveInfo>
{
    std::vector<DriveInfo> driveInfos;
    auto drives = GetDrives();
    for (const auto& drive : drives)
    {
        auto driveType = GetDriveTypeA(drive.c_str());

        if (!checkAccessible(drive, std::chrono::milliseconds(100))) {
            std::cout << "Drive: " << drive << " is not accessible (timed out or not connected)." << std::endl;
            continue; // Skip inaccessible drives
        }

         try {
             auto spaceInfo = std::filesystem::space(drive);
             DriveInfo info;
             info.name = drive;
             info.type = GetDriveTypeStr(driveType);
             info.totalSpace = spaceInfo.capacity;
             info.freeSpace = spaceInfo.free;
             driveInfos.push_back(info);
             PrintInfo(info);
          
         } catch (const std::filesystem::filesystem_error& e) {
             std::cerr << "Error accessing drive " << drive << ": " << e.what() << std::endl;
             continue; // Skip inaccessible drives
         }
          
    }
    return driveInfos;
}

bool DriveChecker3::checkAccessible(const std::string& path, std::chrono::milliseconds timeout)
{
    std::scoped_lock lock(mutex_);

    auto& state = states_[path];
    if (state.future.has_value() &&
        state.future->wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        return false; // 前回の確認がまだ終わっているのでスキップ
    }

    state.future = std::async(std::launch::async, [path]() {
        try {
            return std::filesystem::exists(path);
        }
        catch (...) {
            return false;
        }
        });

    if (state.future->wait_for(timeout) == std::future_status::ready) {
        bool result = state.future->get();
        states_.erase(path); // 完了したら状態を削除
        return result;
    }

    return false; // タイムアウト
}