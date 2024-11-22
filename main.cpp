#include <iostream>

#include "async.h"

int main()
{
	std::cout << "Hello World!" << std::endl;
	
	TestAsync();

	TestPromise();

	TestProducerConsumer();
	
	return 0;
}
