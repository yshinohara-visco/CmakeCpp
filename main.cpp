#include <iostream>
#include "math_operations.h"

int main()
{
    std::cout << "=== Math Operations Library Demo ===" << std::endl;
    
    // Math Operations Library Test
    {
        std::cout << "\n[Math Operations Library Test]" << std::endl;
        
        int a = 10, b = 5;
        std::cout << "a = " << a << ", b = " << b << std::endl;
        
        std::cout << "Addition: " << a << " + " << b << " = " << MathOperations::add(a, b) << std::endl;
        std::cout << "Subtraction: " << a << " - " << b << " = " << MathOperations::subtract(a, b) << std::endl;
        std::cout << "Multiplication: " << a << " * " << b << " = " << MathOperations::multiply(a, b) << std::endl;
        std::cout << "Division: " << a << " / " << b << " = " << MathOperations::divide(a, b) << std::endl;
        
        // Test division by zero exception
        try {
            std::cout << "Division by zero test: " << a << " / 0 = ";
            MathOperations::divide(a, 0);
        } catch (const std::exception& e) {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }
    }
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}
