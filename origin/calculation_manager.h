#pragma once
#include <vector>

namespace origin {

class Logger;

class CalculationManager
{
public:
    CalculationManager(Logger& logger);
    ~CalculationManager();

    double calculate();

private:
    Logger& logger_;
    std::vector<double> data_;

    void initializeData();
    double sum() const;
};

} // namespace origin
