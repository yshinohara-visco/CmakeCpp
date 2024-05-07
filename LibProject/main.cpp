#include <iostream>

#include "ModuleStatic.h"
#include "ModuleShared.h"

int main()
{
	std::cout << "---Static---" << std::endl;
	
	std::cout << StaticFunction(0) << std::endl;

	auto staticClass = StaticClass();
	std::cout << staticClass.MemberFunction(1) << std::endl;

	std::cout << "---Shared---" << std::endl;

	std::cout << SharedFunction(2) << std::endl;
	std::cout << CallStaticFunction(3) << std::endl;

	auto sharedClass = SharedClass();
	std::cout << sharedClass.MemberFunction(4) << std::endl;
	
	return 0;
}
