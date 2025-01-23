#include "ModuleShared.h"
#include "ModuleStatic.h"

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

    StaticClass staticClass;
    staticClass.MemberFunction(1);
}
