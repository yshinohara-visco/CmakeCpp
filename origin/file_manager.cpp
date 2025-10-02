#include "file_manager.h"
#include "logger.h"
#include <fstream>
#include <iostream>

namespace origin {

FileManager::FileManager(const std::string& sessionId, Logger& logger)
    : sessionId_(sessionId)
    , logger_(logger)
{
    logger_.log("FileManager: Initialized with session ID: " + sessionId);
}

FileManager::~FileManager() = default;

std::string FileManager::formatContent(const std::string& content) const
{
    return "=== Processing Results ===\n"
           "Session ID: " + sessionId_ + "\n" +
           content +
           "======================\n";
}

void FileManager::writeToFile(const std::string& path, const std::string& content)
{
    logger_.log("FileManager: Writing to file: " + path);

    std::ofstream file(path);
    if (!file.is_open())
    {
        logger_.log("FileManager: ERROR - Failed to open file: " + path);
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }

    file << formatContent(content);
    file.close();

    logger_.log("FileManager: File write completed");
    std::cout << "Results written to: " << path << std::endl;
}

} // namespace origin
