#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

int main()
{
	std::mutex mtx;
	std::condition_variable cv;

	// 単にwaitしたら、notifyされるまで待つ
	auto th = std::thread([&]
		{
            std::unique_lock<std::mutex> lock(mtx);
			std::cout << "before wait" << std::endl;
            cv.wait(lock);
            std::cout << "after wait" << std::endl;
		});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::cout << "before notify" << std::endl;
	cv.notify_one();
	std::cout << "after notify" << std::endl;
	th.join();

	std::cout << "-----------------------------" << std::endl;

	// 述語付きのwaitは、notifyされた時に述語が新なら抜ける
	std::atomic_bool flag = false;
	th = std::thread([&]
		{
			std::unique_lock<std::mutex> lock(mtx);
            std::cout << "before wait" << std::endl;
            cv.wait(lock, [&]{ return flag.load(); });
            std::cout << "after wait" << std::endl;
		});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::cout << "before notify false" << std::endl;
	cv.notify_one(); //このnotifyでは動かない
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	flag = true;
	std::cout << "before notify true" << std::endl;
	cv.notify_one();
	std::cout << "after notify true" << std::endl;
	th.join();

	std::cout << "-----------------------------" << std::endl;

	// 述語がtrueを返す状態でwaitしたら、止まらない
	// データを入れたらnotifyする系で、データが先に入っても述語がtrueを返すようにしたらOK
	flag = true;
	th = std::thread([&]
	    {
		    std::unique_lock<std::mutex> lock(mtx);
		    std::cout << "before wait" << std::endl;
		    cv.wait(lock, [&]{ return flag.load(); });
		    std::cout << "after wait" << std::endl;
		});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::cout << "before notify true" << std::endl;
	cv.notify_one();
	std::cout << "after notify true" << std::endl;
	th.join();

	std::cout << "-----------------------------" << std::endl;

	// wait_forでタイムアウトもできる
	flag = false;
	th = std::thread([&]
	    {
		    std::unique_lock<std::mutex> lock(mtx);
		    std::cout << "before wait" << std::endl;
		    if(cv.wait_for(lock, std::chrono::milliseconds(100), [&]{ return flag.load(); }))
		    {
				std::cout << "after wait" << std::endl;
			}
		    else
		    {
				std::cout << "timeout" << std::endl;
			}
		});
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	flag = true;
	std::cout << "before notify true" << std::endl;
	cv.notify_one();
	std::cout << "after notify true" << std::endl;
	th.join();


	
	return 0;
}
