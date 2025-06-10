#pragma once

#include "common.h"

/*
全体的に非同期にして、更新指示→終わるか一定時間経過でその時点の結果を取得、とかどうか
*/
class DriveChecker3
{
public:
    DriveChecker3();
    ~DriveChecker3();

    auto GetAllDriveSpace() -> std::vector<DriveInfo>;


private:

};