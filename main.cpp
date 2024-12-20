#include <iostream>

#include "precise_wait.h"
#include "stop_watch.h"

int main()
{
	std::cout << "Hello World!" << std::endl;


	StopWatch sw;


	/*
	現在の時間を取得するのにかかる時間は？
	*/
	auto get_now = [&]
		{
            for (size_t i = 0; i < 10; i++)
            {
			    sw.Start("--GetNow");
			    auto now = std::chrono::high_resolution_clock::now();
			    sw.Print();
			}
		};

	/*
	スピンウェイト
	50回に1回くらいの頻度で3ms程度の誤差が発生する
	*/
	auto spin_wait = [&]
		{
			for (size_t i = 0; i < 10; i++)
			{
				sw.Start("--PreciseWait");
				PreciseWait(std::chrono::milliseconds(1));
				sw.Print();
			}
		};

	/*
	スピンウェイトを用いた非同期タスク実行
	50回に2回くらいの頻度で2～10ms程度の誤差が発生する
	*/
	auto post_task = [&]
		{
			PreciseWaiter pw;
			for (size_t i = 0; i < 10; i++)
			{
				StopWatchG::Start("--PostTask");
				pw.PostTask(std::chrono::milliseconds(1), [&] {
					StopWatchG::Wrap("do task   ");
					});
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
		};

	/*
	timeBeginPeriod(1), timeEndPeriod(1)で精度を上げたスリープ
	基本的に1ms後半で、5msくらいまで誤差が発生する
	*/
	auto period_wait = [&]
        {
		    for (size_t i = 0; i < 10; i++)
		    {
				sw.Start("--PeriodWait");
				PeriodWait(std::chrono::milliseconds(1));
				sw.Print();
			}
		};

	/*
	WaitableTimerを用いたウェイト
	基本的に16msの倍数になる???? 精度悪すぎでは？
	*/
	auto waitable_wait = [&]
		{
			for (size_t i = 0; i < 20; i++)
			{
				sw.Start("--WaitableWait");
				WaitableWait(std::chrono::milliseconds(1));
				sw.Print();
			}
		};

	/*
	WaitableTimerを用いたウェイト
	timeBeginPeriodとの併用で最大3ms程度にはなったが今度は1ms未満が出てきた
	*/
	auto waitable_period = [&]
        {
			timeBeginPeriod(1);
			for (size_t i = 0; i < 20; i++)
			{
				sw.Start("--Waitable period");
				WaitableWait(std::chrono::milliseconds(1));
				sw.Print();
			}
			timeEndPeriod(1);
		};

	/*
	WaitableTImerを用いた非同期タスク実行
	基本的に16msの倍数になる???? 精度悪すぎでは？
	*/
	auto waitable_post = [&]
		{
			Waitable w;
			for (size_t i = 0; i < 5; i++)
			{
				StopWatchG::Start("--Waitable post");
				w.PostTask(std::chrono::milliseconds(1), [&] {
					StopWatchG::Wrap("do task   ");
					});
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
		};

	auto wait_timeout = [&]
		{
			auto timer = CreateWaitableTimer(nullptr, FALSE, nullptr);
			for (size_t i = 0; i < 10; i++)
			{
				sw.Start("--Wait timuout");
				WaitForSingleObject(timer, 1);
				sw.Print();
			}
			CloseHandle(timer);
		};

	auto wait_timeout_period = [&]
		{
			timeBeginPeriod(1);
            auto timer = CreateWaitableTimer(nullptr, FALSE, nullptr);
            for (size_t i = 0; i < 20; i++)
            {
			    sw.Start("--Wait timuout period");
			    WaitForSingleObject(timer, 1);
			    sw.Print();
			}
            CloseHandle(timer);
            timeEndPeriod(1);
        };
	
	get_now();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "----------------------------" << std::endl;

	spin_wait();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "----------------------------" << std::endl;

	post_task();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "----------------------------" << std::endl;

	period_wait();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "----------------------------" << std::endl;

	waitable_wait();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "----------------------------" << std::endl;

	waitable_period();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "----------------------------" << std::endl;

	waitable_post();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "----------------------------" << std::endl;

	wait_timeout();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "----------------------------" << std::endl;

	wait_timeout_period();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "----------------------------" << std::endl;

	return 0;
}
