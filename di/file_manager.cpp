#include "file_manager.h"
#include "logger.h"
#include <fstream>
#include <iostream>

namespace di {

FileManager::FileManager(std::shared_ptr<Logger> logger)
    : logger_(logger)
{
    logger_->log("FileManager: Initialized");
}

FileManager::~FileManager() = default;

void FileManager::setSessionId(const std::string& sessionId)
{
    sessionId_ = sessionId;
    logger_->log("FileManager: Session ID set to: " + sessionId);
}

std::string FileManager::formatContent(const std::string& content) const
{
    return "=== DI Processing Results ===\n"
           "Session ID: " + sessionId_ + "\n" +
           content +
           "=============================\n";
}

void FileManager::writeToFile(const std::string& path, const std::string& content)
{
    logger_->log("FileManager: Writing to file: " + path);

    std::ofstream file(path);
    if (!file.is_open())
    {
        logger_->log("FileManager: ERROR - Failed to open file: " + path);
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }

    file << formatContent(content);
    file.close();

    logger_->log("FileManager: File write completed");
    std::cout << "Results written to: " << path << std::endl;
}

} // namespace di
