#include "data_manager.h"
#include "calculation_manager.h"
#include "statistics_manager.h"
#include "logger.h"
#include <sstream>

namespace origin {

DataManager::DataManager(Logger& logger)
    : logger_(logger)
    , calculationManager_(std::make_unique<CalculationManager>(logger))
    , statisticsManager_(std::make_unique<StatisticsManager>(logger))
{
    logger_.log("DataManager: Initialized");
}

DataManager::~DataManager() = default;

std::string DataManager::process()
{
    logger_.log("DataManager: Starting data processing");

    auto calcResult = calculationManager_->calculate();
    auto statsResult = statisticsManager_->analyze(calcResult);

    std::ostringstream oss;
    oss << "Calculation Result: " << calcResult << "\n";
    oss << "Statistics Result: " << statsResult << "\n";

    logger_.log("DataManager: Data processing completed");
    return oss.str();
}

} // namespace origin
