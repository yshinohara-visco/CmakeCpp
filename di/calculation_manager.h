#pragma once
#include <vector>
#include <memory>

namespace di {

class Logger;

class CalculationManager
{
public:
    CalculationManager(std::shared_ptr<Logger> logger);
    ~CalculationManager();

    double calculate();

private:
    std::shared_ptr<Logger> logger_;
    std::vector<double> data_;

    void initializeData();
    double sum() const;
};

} // namespace di
