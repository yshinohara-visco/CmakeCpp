#include "ModuleStatic.h"

#include <iostream>

std::string StaticFunction(int num)
{
    std::cout << "StaticFunction(" << num << ")" << std::endl;
    return "StaticFunction: " + std::to_string(num);
}

StaticClass::StaticClass()
{
    std::cout << "StaticClass::StaticClass()" << std::endl;
}

std::string StaticClass::MemberFunction(int num)
{
    std::cout << "StaticClass::MemberFunction(" << num << ")" << std::endl;
    return "StaticClass::MemberFunction: " + std::to_string(num);
}