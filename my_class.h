#pragma once

#include <iostream>
#include <string>

// コンストラクタとデストラクタの追跡用
class MyClass
{
public:
	MyClass(std::string str)
		: str_(str)
	{
		std::cout << "Construct: " << str_ << std::endl;
	}

	~MyClass()
	{
		std::cout << "Destruct: " << str_ << std::endl;
	}

private:
	std::string str_;
};