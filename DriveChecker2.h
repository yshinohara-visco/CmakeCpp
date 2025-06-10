#pragma once

#include "common.h"

/*
いきなりGetDiskFreeSpaceExAで容量を取りに行く前に、「接続しているか」を判定したら安定する？
その判定にかかる時間が不定なのであんまり意味が無かった
*/
class DriveChecker2
{
public:
    DriveChecker2();
    ~DriveChecker2();

    auto GetAllDriveSpace() -> std::vector<DriveInfo>;


private:

};