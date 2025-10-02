#pragma once
#include <string>

namespace origin {

class Logger;

class FileManager
{
public:
    FileManager(const std::string& sessionId, Logger& logger);
    ~FileManager();

    void writeToFile(const std::string& path, const std::string& content);

private:
    std::string sessionId_;
    Logger& logger_;

    std::string formatContent(const std::string& content) const;
};

} // namespace origin
