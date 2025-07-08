#include <gtest/gtest.h>
#include "math_operations.h"

// 加算のテスト
TEST(MathOperationsTest, Addition) {
    EXPECT_EQ(MathOperations::add(2, 3), 5);
    EXPECT_EQ(MathOperations::add(-1, 1), 0);
    EXPECT_EQ(MathOperations::add(-5, -3), -8);
    EXPECT_EQ(MathOperations::add(0, 0), 0);
}

// 減算のテスト
TEST(MathOperationsTest, Subtraction) {
    EXPECT_EQ(MathOperations::subtract(5, 3), 2);
    EXPECT_EQ(MathOperations::subtract(1, 1), 0);
    EXPECT_EQ(MathOperations::subtract(-2, -5), 3);
    EXPECT_EQ(MathOperations::subtract(0, 5), -5);
}

// 乗算のテスト
TEST(MathOperationsTest, Multiplication) {
    EXPECT_EQ(MathOperations::multiply(3, 4), 12);
    EXPECT_EQ(MathOperations::multiply(-2, 3), -6);
    EXPECT_EQ(MathOperations::multiply(-2, -3), 6);
    EXPECT_EQ(MathOperations::multiply(0, 100), 0);
    EXPECT_EQ(MathOperations::multiply(1, 7), 7);
}

// 除算のテスト
TEST(MathOperationsTest, Division) {
    EXPECT_EQ(MathOperations::divide(10, 2), 5);
    EXPECT_EQ(MathOperations::divide(9, 3), 3);
    EXPECT_EQ(MathOperations::divide(-8, 2), -4);
    EXPECT_EQ(MathOperations::divide(-6, -2), 3);
    EXPECT_EQ(MathOperations::divide(0, 5), 0);
}

// ゼロ除算のテスト
TEST(MathOperationsTest, DivisionByZero) {
    EXPECT_THROW(MathOperations::divide(5, 0), std::invalid_argument);
    EXPECT_THROW(MathOperations::divide(-3, 0), std::invalid_argument);
    EXPECT_THROW(MathOperations::divide(0, 0), std::invalid_argument);
}

// 境界値のテスト
TEST(MathOperationsTest, BoundaryValues) {
    // 最大値付近のテスト
    EXPECT_EQ(MathOperations::add(INT_MAX - 1, 1), INT_MAX);
    EXPECT_EQ(MathOperations::subtract(INT_MIN + 1, 1), INT_MIN);
    
    // 1での除算
    EXPECT_EQ(MathOperations::divide(42, 1), 42);
    EXPECT_EQ(MathOperations::divide(-42, 1), -42);
    
    // -1での除算
    EXPECT_EQ(MathOperations::divide(42, -1), -42);
    EXPECT_EQ(MathOperations::divide(-42, -1), 42);
}