#include <iostream>

#include "coroutine.h"

int main()
{
	// 中断、再開を繰り返し最後まで実行
	{
		auto task = DoTasks();
		std::cout << "main 0" << std::endl;
		task.resume(); // initial->Do task 1
		std::cout << "main 1" << std::endl;
		task.resume(); // Do task 2->Do task 3
		std::cout << "main 2" << std::endl;
		task.resume(); // Do task 3 -> final_suspend
		std::cout << "main 3" << std::endl;
		//これだけでは非同期とかは無く、一つずつ実行している状態
	}

	// 途中で放棄
	{
		auto task = DoTasks();
		task.resume();
		task.resume();
		// 途中で終了
	}

	// 長い処理を別スレッドから終了
	{
		auto task = LongTasks();
		//　繰り返しresume
		auto th1 = std::thread([&task](){
			while(task.is_suspend())
			{
				task.resume();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			std::cout << "thread 1 done" << std::endl;
		});
		// 3秒後に終了
		auto th2 = std::thread([&task](){
			std::this_thread::sleep_for(std::chrono::seconds(3));
			task.abort();
			std::cout << "thread 2 done" << std::endl;
		});
		th1.join();
		th2.join();
	}
	
	return 0;
}
