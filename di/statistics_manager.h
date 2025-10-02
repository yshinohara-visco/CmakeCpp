#pragma once
#include <memory>

namespace di {

class Logger;

class StatisticsManager
{
public:
    StatisticsManager(std::shared_ptr<Logger> logger);
    ~StatisticsManager();

    double analyze(double value);

private:
    std::shared_ptr<Logger> logger_;
    int analysisCount_;
    double totalAnalyzed_;

    double calculateAverage() const;
};

} // namespace di
