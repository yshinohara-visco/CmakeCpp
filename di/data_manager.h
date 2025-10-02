#pragma once
#include <memory>
#include <string>
#include "calculation_manager.h"
#include "statistics_manager.h"
#include "logger.h"

namespace di {

class DataManager
{
public:
    // DIコンテナから依存オブジェクトを注入
    DataManager(
        std::shared_ptr<Logger> logger,
        std::shared_ptr<CalculationManager> calculationManager,
        std::shared_ptr<StatisticsManager> statisticsManager
    );
    ~DataManager();

    std::string process();

private:
    std::shared_ptr<Logger> logger_;
    std::shared_ptr<CalculationManager> calculationManager_;
    std::shared_ptr<StatisticsManager> statisticsManager_;
};

} // namespace di
