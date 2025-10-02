#include "data_manager.h"
#include <sstream>

namespace di {

DataManager::DataManager(
    std::shared_ptr<Logger> logger,
    std::shared_ptr<CalculationManager> calculationManager,
    std::shared_ptr<StatisticsManager> statisticsManager
)
    : logger_(logger)
    , calculationManager_(calculationManager)
    , statisticsManager_(statisticsManager)
{
    logger_->log("DataManager: Initialized");
}

DataManager::~DataManager() = default;

std::string DataManager::process()
{
    logger_->log("DataManager: Starting data processing");

    auto calcResult = calculationManager_->calculate();
    auto statsResult = statisticsManager_->analyze(calcResult);

    std::ostringstream oss;
    oss << "Calculation Result: " << calcResult << "\n";
    oss << "Statistics Result: " << statsResult << "\n";

    logger_->log("DataManager: Data processing completed");
    return oss.str();
}

} // namespace di
