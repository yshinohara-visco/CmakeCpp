#include "ModuleShared.h"

#include <iostream>

std::string SharedFunction( int num )
{
    std::cout << "SharedFunction(" << num << ")" << std::endl;
    return "SharedFunction: " + std::to_string(num);
}

std::string CallStaticFunction(int num)
{
    std::cout << "CallStaticFunction(" << num << ")" << std::endl;
    return StaticFunction(num);
}

SharedClass::SharedClass()
{
    std::cout << "SharedClass::SharedClass()" << std::endl;
}

std::string SharedClass::MemberFunction(int num)
{
    StaticClass::MemberFunction(num);

    std::cout << "SharedClass::MemberFunction(" << num << ")" << std::endl;
    return "SharedClass::MemberFunction";
}