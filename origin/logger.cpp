#include "logger.h"

namespace origin {

Logger::Logger()
{
    std::cout << "[Logger] Initialized" << std::endl;
}

Logger::~Logger() = default;

void Logger::output(const std::string& message) const
{
    std::cout << "[LOG] " << message << std::endl;
}

void Logger::log(const std::string& message) const
{
    output(message);
}

} // namespace origin
