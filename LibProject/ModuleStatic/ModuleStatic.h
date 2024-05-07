#pragma once

#include <string>

std::string StaticFunction( int num );

class StaticClass
{
public:
    StaticClass();

    virtual std::string MemberFunction( int num );
};