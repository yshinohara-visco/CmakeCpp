#pragma once
#include <memory>
#include <string>

namespace origin {

class CalculationManager;
class StatisticsManager;
class Logger;

class DataManager
{
public:
    DataManager(Logger& logger);
    ~DataManager();

    std::string process();

private:
    Logger& logger_;
    std::unique_ptr<CalculationManager> calculationManager_;
    std::unique_ptr<StatisticsManager> statisticsManager_;
};

} // namespace origin
