#include "hello.h"

#include <iostream>

void SayHello()
{
    std::cout << "Hello World!" << std::endl;
}

void SayMoreMoreHello()
{
#ifdef WIN32
    std::cout << "Hello Hello Hello! windows" << std::endl;
#elif
    std::cout << "Hello Hello Hello! unix" << std::endl;
#endif // WIN32
}