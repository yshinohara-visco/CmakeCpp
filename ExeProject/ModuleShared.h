#pragma once

#include "ModuleStatic.h"

std::string SharedFunction(int num);
std::string CallStaticFunction(int num);

class SharedClass : public StaticClass
{
public:
    SharedClass();

    std::string MemberFunction(int num) override;
};