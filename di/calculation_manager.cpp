#include "calculation_manager.h"
#include "logger.h"
#include <numeric>
#include <sstream>

namespace di {

CalculationManager::CalculationManager(std::shared_ptr<Logger> logger)
    : logger_(logger)
{
    initializeData();
    logger_->log("CalculationManager: Initialized");
}

CalculationManager::~CalculationManager() = default;

void CalculationManager::initializeData()
{
    data_ = {10.5, 20.3, 30.7, 40.2, 50.9};
}

double CalculationManager::sum() const
{
    return std::accumulate(data_.begin(), data_.end(), 0.0);
}

double CalculationManager::calculate()
{
    logger_->log("CalculationManager: Calculating sum");
    auto result = sum();

    std::ostringstream oss;
    oss << "CalculationManager: Result = " << result;
    logger_->log(oss.str());

    return result;
}

} // namespace di
