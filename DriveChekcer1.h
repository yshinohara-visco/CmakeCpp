#pragma once

#include "common.h"

/*
�P��GetLogicalDriveStringsA�œ���ꂽ�h���C�u�ɑ΂���GetDriveTypeA��GetDiskFreeSpaceExA�ŏ����擾����ꍇ
�l�b�g���[�N�h���C�u�Ɍq����Ȃ����ɒ����Ԃ�����ꍇ������
*/
class DriveChecker1
{
public:
    DriveChecker1();
    ~DriveChecker1();

    auto GetAllDriveSpace() -> std::vector<DriveInfo>;


private:

};