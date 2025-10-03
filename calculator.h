#pragma once
#include <cmath>
#include <stdexcept>

class Calculator {
public:
    // 加算
    double add(double a, double b) const {
        return a + b;
    }

    // 減算
    double subtract(double a, double b) const {
        return a - b;
    }

    // 乗算
    double multiply(double a, double b) const {
        return a * b;
    }

    // 除算
    double divide(double a, double b) const {
        if (b == 0.0) {
            throw std::invalid_argument("Division by zero");
        }
        return a / b;
    }

    // 平方根
    double sqrt(double a) const {
        if (a < 0.0) {
            throw std::invalid_argument("Square root of negative number");
        }
        return std::sqrt(a);
    }

    // 累乗
    double power(double base, double exponent) const {
        return std::pow(base, exponent);
    }
};
