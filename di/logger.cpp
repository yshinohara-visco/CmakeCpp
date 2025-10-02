#include "logger.h"

namespace di {

Logger::Logger()
{
    std::cout << "[Logger] Initialized (DI)" << std::endl;
}

Logger::~Logger() = default;

void Logger::output(const std::string& message) const
{
    std::cout << "[LOG-DI] " << message << std::endl;
}

void Logger::log(const std::string& message) const
{
    output(message);
}

} // namespace di
