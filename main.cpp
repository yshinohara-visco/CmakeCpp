#include <iostream>

#include "DriveChekcer1.h"
#include "DriveChecker2.h"

int main()
{
	std::cout << "Hello World!" << std::endl;

	//DriveChecker1 dc1;
	//dc1.GetAllDriveSpace();

    DriveChecker2 dc2;
    auto driveInfos = dc2.GetAllDriveSpace();


	return 0;
}
