#include "DriveChecker3.h"

#include <iostream>
#include "windows.h"

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
    std::unique_lock lock(mutex_);

    if(checking_[path])
    {
        return false; // 前回の処理が終わっていないのでスキップ
    }

    checking_[path] = true;
    connected_[path] = false;

    std::weak_ptr<DriveChecker3> weak_this = weak_from_this();
    auto th = std::thread([weak_this, path](){
        bool result = false;
        try{
            result = std::filesystem::exists(path);
        }
        catch(...){
            result = false;
        }
        if(auto shared_this = weak_this.lock())
        {
            std::unique_lock lock(shared_this->mutex_);
            shared_this->connected_[path] = result;
            shared_this->checking_[path] = false;
            shared_this->cv_.notify_all(); // 通知待機中のスレッド
        }
    });
    th.detach();
    // detachしようがプログラム終了時に長引くパターンを引いてしまうと終了しなくなってしまう

    // 確認が終わるかタイムアウトまで待つ
    cv_.wait_for(lock, timeout, [this, &path]() {
        return !checking_[path];
        });

    return connected_[path];
}