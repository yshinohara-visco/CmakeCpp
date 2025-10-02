#pragma once
#include <string>
#include <memory>

namespace di {

class Logger;

class FileManager
{
public:
    FileManager(std::shared_ptr<Logger> logger);
    ~FileManager();

    void setSessionId(const std::string& sessionId);
    void writeToFile(const std::string& path, const std::string& content);

private:
    std::string sessionId_;
    std::shared_ptr<Logger> logger_;

    std::string formatContent(const std::string& content) const;
};

} // namespace di
