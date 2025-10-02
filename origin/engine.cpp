#include "engine.h"
#include "data_manager.h"
#include "file_manager.h"
#include "logger.h"
#include <chrono>
#include <sstream>

namespace origin {

Engine::Engine()
    : logger_(std::make_unique<Logger>())
{
    logger_->log("Engine: Starting initialization");
    dataManager_ = std::make_unique<DataManager>(*logger_);

    auto initConfig = initialize();
    fileManager_ = std::make_unique<FileManager>(initConfig, *logger_);

    logger_->log("Engine: Initialization completed");
}

Engine::~Engine() = default;

std::string Engine::initialize()
{
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << "Engine_" << timestamp;

    logger_->log("Engine: Session ID generated: " + oss.str());
    return oss.str();
}

void Engine::run(const std::string& outputPath)
{
    logger_->log("Engine: Starting processing");
    auto result = dataManager_->process();
    fileManager_->writeToFile(outputPath, result);
    logger_->log("Engine: Processing completed");
}

} // namespace origin
