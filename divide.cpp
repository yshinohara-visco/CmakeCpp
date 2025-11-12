#include "divide.h"

#include <stdexcept>

double divide1(int a, int b)
{
    if (b == 0)
    {
        throw std::runtime_error("Division by zero");
    }
    return static_cast<double>(a) / b;
}

bool divide2(int a, int b, double& result)
{
    if (b == 0)
    {
        return false;
    }
    result = static_cast<double>(a) / b;
    return true;
}

std::optional<double> divide3(int a, int b)
{
    if (b == 0)
    {
        return std::nullopt;
    }
    return static_cast<double>(a) / b;
}

std::variant<double,std::string> divide4(int a, int b)
{
    if (b == 0)
    {
        return "Division by zero";
    }
    return static_cast<double>(a) / b;
}

std::expected<double, std::string> divide5(int a, int b)
{
    if (b == 0)
    {
        return std::unexpected("Division by zero");
    }
    return static_cast<double>(a) / b;
}