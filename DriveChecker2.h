#pragma once

#include "common.h"

/*
�����Ȃ�GetDiskFreeSpaceExA�ŗe�ʂ����ɍs���O�ɁA�u�ڑ����Ă��邩�v�𔻒肵������肷��H
���̔���ɂ����鎞�Ԃ��s��Ȃ̂ł���܂�Ӗ�����������
*/
class DriveChecker2
{
public:
    DriveChecker2();
    ~DriveChecker2();

    auto GetAllDriveSpace() -> std::vector<DriveInfo>;


private:

};