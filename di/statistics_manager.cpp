#include "statistics_manager.h"
#include "logger.h"
#include <sstream>

namespace di {

StatisticsManager::StatisticsManager(std::shared_ptr<Logger> logger)
    : logger_(logger)
    , analysisCount_(0)
    , totalAnalyzed_(0.0)
{
    logger_->log("StatisticsManager: Initialized");
}

StatisticsManager::~StatisticsManager() = default;

double StatisticsManager::calculateAverage() const
{
    if (analysisCount_ == 0) return 0.0;
    return totalAnalyzed_ / analysisCount_;
}

double StatisticsManager::analyze(double value)
{
    logger_->log("StatisticsManager: Analyzing value");

    analysisCount_++;
    totalAnalyzed_ += value;
    auto result = calculateAverage();

    std::ostringstream oss;
    oss << "StatisticsManager: Average = " << result;
    logger_->log(oss.str());

    return result;
}

} // namespace di
