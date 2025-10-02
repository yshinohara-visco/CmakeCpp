#include "engine.h"
#include <sstream>

namespace di {

Engine::Engine(
    std::shared_ptr<Logger> logger,
    std::shared_ptr<DataManager> dataManager,
    std::shared_ptr<FileManager> fileManager
)
    : logger_(logger)
    , dataManager_(dataManager)
    , fileManager_(fileManager)
{
    logger_->log("Engine: Initialized");

    // Engineが初期化処理を行い、その結果をFileManagerに設定
    auto sessionId = generateSessionId();
    fileManager_->setSessionId(sessionId);
}

Engine::~Engine() = default;

std::string Engine::generateSessionId()
{
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << "Engine_DI_" << timestamp;

    logger_->log("Engine: Generated session ID: " + oss.str());
    return oss.str();
}

void Engine::run(const std::string& outputPath)
{
    logger_->log("Engine: Starting processing");
    auto result = dataManager_->process();
    fileManager_->writeToFile(outputPath, result);
    logger_->log("Engine: Processing completed");
}

} // namespace di
