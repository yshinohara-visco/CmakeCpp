#include <iostream>
#include <stdexcept>
#include "../calculator.h"

int main() {
    Calculator calc;
    int passed = 0;
    int failed = 0;

    // 加算テスト
    std::cout << "[TEST] Addition tests..." << std::endl;
    if (calc.add(2, 3) == 5) {
        std::cout << "  PASS: 2 + 3 = 5" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: 2 + 3 != 5" << std::endl;
        failed++;
    }

    if (calc.add(-1, 1) == 0) {
        std::cout << "  PASS: -1 + 1 = 0" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: -1 + 1 != 0" << std::endl;
        failed++;
    }

    // 減算テスト
    std::cout << "[TEST] Subtraction tests..." << std::endl;
    if (calc.subtract(5, 3) == 2) {
        std::cout << "  PASS: 5 - 3 = 2" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: 5 - 3 != 2" << std::endl;
        failed++;
    }

    if (calc.subtract(0, 5) == -5) {
        std::cout << "  PASS: 0 - 5 = -5" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: 0 - 5 != -5" << std::endl;
        failed++;
    }

    // 乗算テスト
    std::cout << "[TEST] Multiplication tests..." << std::endl;
    if (calc.multiply(2, 3) == 6) {
        std::cout << "  PASS: 2 * 3 = 6" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: 2 * 3 != 6" << std::endl;
        failed++;
    }

    if (calc.multiply(-2, 3) == -6) {
        std::cout << "  PASS: -2 * 3 = -6" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: -2 * 3 != -6" << std::endl;
        failed++;
    }

    // 除算テスト
    std::cout << "[TEST] Division tests..." << std::endl;
    if (calc.divide(6, 2) == 3) {
        std::cout << "  PASS: 6 / 2 = 3" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: 6 / 2 != 3" << std::endl;
        failed++;
    }

    if (calc.divide(5, 2) == 2.5) {
        std::cout << "  PASS: 5 / 2 = 2.5" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: 5 / 2 != 2.5" << std::endl;
        failed++;
    }

    // ゼロ除算テスト
    std::cout << "[TEST] Division by zero test..." << std::endl;
    try {
        calc.divide(5, 0);
        std::cout << "  FAIL: Division by zero should throw exception" << std::endl;
        failed++;
    } catch (const std::invalid_argument&) {
        std::cout << "  PASS: Division by zero throws exception" << std::endl;
        passed++;
    }

    // 平方根テスト
    std::cout << "[TEST] Square root tests..." << std::endl;
    if (calc.sqrt(4) == 2) {
        std::cout << "  PASS: sqrt(4) = 2" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: sqrt(4) != 2" << std::endl;
        failed++;
    }

    if (calc.sqrt(9) == 3) {
        std::cout << "  PASS: sqrt(9) = 3" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: sqrt(9) != 3" << std::endl;
        failed++;
    }

    if (calc.sqrt(0) == 0) {
        std::cout << "  PASS: sqrt(0) = 0" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: sqrt(0) != 0" << std::endl;
        failed++;
    }

    // 負の数の平方根テスト
    std::cout << "[TEST] Square root of negative number test..." << std::endl;
    try {
        calc.sqrt(-1);
        std::cout << "  FAIL: sqrt(-1) should throw exception" << std::endl;
        failed++;
    } catch (const std::invalid_argument&) {
        std::cout << "  PASS: sqrt(-1) throws exception" << std::endl;
        passed++;
    }

    // 累乗テスト
    std::cout << "[TEST] Power tests..." << std::endl;
    if (calc.power(2, 3) == 8) {
        std::cout << "  PASS: 2^3 = 8" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: 2^3 != 8" << std::endl;
        failed++;
    }

    if (calc.power(5, 2) == 25) {
        std::cout << "  PASS: 5^2 = 25" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: 5^2 != 25" << std::endl;
        failed++;
    }

    if (calc.power(10, 0) == 1) {
        std::cout << "  PASS: 10^0 = 1" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: 10^0 != 1" << std::endl;
        failed++;
    }

    if (calc.power(2, -1) == 0.5) {
        std::cout << "  PASS: 2^-1 = 0.5" << std::endl;
        passed++;
    } else {
        std::cout << "  FAIL: 2^-1 != 0.5" << std::endl;
        failed++;
    }

    // 結果サマリー
    std::cout << "\n========================================" << std::endl;
    std::cout << "Test Results: " << passed << " passed, " << failed << " failed" << std::endl;
    std::cout << "========================================" << std::endl;

    return (failed == 0) ? 0 : 1;
}
