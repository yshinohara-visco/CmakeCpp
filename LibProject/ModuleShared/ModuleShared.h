#pragma once

#include <string>

std::string SharedFunction(int num);
std::string CallStaticFunction(int num);

class SharedClass
{
public:
    SharedClass();
};