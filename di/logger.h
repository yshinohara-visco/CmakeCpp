#pragma once
#include <string>
#include <iostream>

namespace di {

class Logger
{
public:
    Logger();
    ~Logger();

    void log(const std::string& message) const;

private:
    void output(const std::string& message) const;
};

} // namespace di
