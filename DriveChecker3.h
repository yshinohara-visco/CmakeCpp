#pragma once

#include "common.h"

/*
�S�̓I�ɔ񓯊��ɂ��āA�X�V�w�����I��邩��莞�Ԍo�߂ł��̎��_�̌��ʂ��擾�A�Ƃ��ǂ���
*/
class DriveChecker3
{
public:
    DriveChecker3();
    ~DriveChecker3();

    auto GetAllDriveSpace() -> std::vector<DriveInfo>;


private:

};