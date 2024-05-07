#include <iostream>

#include "ModuleShared.h"

int main()
{
	std::cout << "---Shared---" << std::endl;

	std::cout << SharedFunction(2) << std::endl;
	std::cout << CallStaticFunction(3) << std::endl;

	auto sharedClass = SharedClass();
	std::cout << sharedClass.MemberFunction(4) << std::endl;
	
	return 0;
}
