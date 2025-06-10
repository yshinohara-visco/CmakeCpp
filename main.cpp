#include <iostream>

#include "DriveChekcer1.h"
#include "DriveChecker2.h"
#include "DriveChecker3.h"

int main()
{
	std::cout << "Hello World!" << std::endl;

	//DriveChecker1 dc1;
	//dc1.GetAllDriveSpace();

    //DriveChecker2 dc2;
    //auto driveInfos = dc2.GetAllDriveSpace();

    DriveChecker3 dc3;
	for (size_t i = 0; i < 10; i++)
	{
		std::cout << "----" << i << "----" << std::endl;
		auto driveInfos = dc3.GetAllDriveSpace();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
    

	return 0;
}
