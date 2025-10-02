#pragma once
#include <memory>
#include <string>

namespace origin {

class DataManager;
class FileManager;
class Logger;

class Engine
{
public:
    Engine();
    ~Engine();

    void run(const std::string& outputPath);

private:
    std::unique_ptr<Logger> logger_;
    std::unique_ptr<DataManager> dataManager_;
    std::unique_ptr<FileManager> fileManager_;

    std::string initialize();
};

} // namespace origin
