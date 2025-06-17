#pragma once

#include "common.h"

/*
単にGetLogicalDriveStringsAで得られたドライブに対してGetDriveTypeAとGetDiskFreeSpaceExAで情報を取得する場合
ネットワークドライブに繋がらない時に長時間かかる場合がある
*/
class DriveChecker1
{
public:
    DriveChecker1();
    ~DriveChecker1();

    auto GetAllDriveSpace() -> std::vector<DriveInfo>;


private:

};