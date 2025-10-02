#pragma once
#include <memory>
#include <string>
#include <chrono>
#include "data_manager.h"
#include "file_manager.h"
#include "logger.h"

namespace di {

class Engine
{
public:
    // DIコンテナから依存オブジェクトを注入
    Engine(
        std::shared_ptr<Logger> logger,
        std::shared_ptr<DataManager> dataManager,
        std::shared_ptr<FileManager> fileManager
    );
    ~Engine();

    void run(const std::string& outputPath);

private:
    std::shared_ptr<Logger> logger_;
    std::shared_ptr<DataManager> dataManager_;
    std::shared_ptr<FileManager> fileManager_;

    std::string generateSessionId();
};

} // namespace di
