#include <iostream>
#include <chrono>

#include "DriveChecker1.h"
#include "DriveChecker2.h"
#include "DriveChecker3.h"
#include "DriveChecker4.h"

int main()
{
	std::cout << "Hello World!" << std::endl;

    auto start = std::chrono::steady_clock::now();

	//DriveChecker1 dc1;
	//dc1.GetAllDriveSpace();
 //   auto p1 = std::chrono::steady_clock::now();
 //   std::cout << "Time taken for DriveChecker1: "
 //       << std::chrono::duration_cast<std::chrono::milliseconds>(p1 - start).count()
 //       << " ms" << std::endl;

 //   DriveChecker2 dc2;
 //   dc2.GetAllDriveSpace();
 //   auto p2 = std::chrono::steady_clock::now();
 //   std::cout << "Time taken for DriveChecker2: "
 //       << std::chrono::duration_cast<std::chrono::milliseconds>(p2 - p1).count()
 //       << " ms" << std::endl;

 //   auto dc3 = DriveChecker3::Create();
	//for (size_t i = 0; i < 1; i++)
	//{
	//	std::cout << "----" << i << "----" << std::endl;
	//	dc3->GetAllDriveSpace();
	//	std::this_thread::sleep_for(std::chrono::seconds(1));
	//}
    auto p3 = std::chrono::steady_clock::now();
    //std::cout << "Time taken for DriveChecker3: "
    //    << std::chrono::duration_cast<std::chrono::milliseconds>(p3 - p2).count()
    //    << " ms" << std::endl;

    DriveChecker4 dc4;
    dc4.GetAllDriveSpace();
    auto p4 = std::chrono::steady_clock::now();
    std::cout << "Time taken for DriveChecker4: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(p4 - p3).count()
        << " ms" << std::endl;

    //何か入力したら終了
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

	return 0;
}
