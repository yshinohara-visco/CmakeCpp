#pragma once

namespace origin {

class Logger;

class StatisticsManager
{
public:
    StatisticsManager(Logger& logger);
    ~StatisticsManager();

    double analyze(double value);

private:
    Logger& logger_;
    int analysisCount_;
    double totalAnalyzed_;

    double calculateAverage() const;
};

} // namespace origin
