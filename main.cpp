#include <iostream>

#include "precise_wait.h"
#include "stop_watch.h"

int main()
{
	std::cout << "Hello World!" << std::endl;


	StopWatch sw;

	/*
	スピンウェイト
	50回に1回くらいの頻度で3ms程度の誤差が発生する
	*/
	for (size_t i = 0; i < 0; i++)
	{
		sw.Start("--PreciseWait");
		PreciseWait(std::chrono::milliseconds(1));
		sw.Print();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	/*
	スピンウェイトを用いた非同期タスク実行
	50回に2回くらいの頻度で2～10ms程度の誤差が発生する
	*/
	std::cout << "----------------------------" << std::endl;
	PreciseWaiter pw;
	for (size_t i = 0; i < 0; i++)
    {
		StopWatchG::Start("--PostTask");
	    pw.PostTask(std::chrono::milliseconds(1), [&] {
			StopWatchG::Wrap("do task   ");
			});
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	/*
	timeBeginPeriod(1), timeEndPeriod(1)で精度を上げたスリープ
	基本的に1ms後半で、5msくらいまで誤差が発生する
	*/
	std::cout << "----------------------------" << std::endl;
	for (size_t i = 0; i < 100; i++)
	{
		sw.Start("--PeriodWait");
		PeriodWait(std::chrono::milliseconds(1));
		sw.Print();
	}

	/*
	WaitableTimerを用いたウェイト
	基本的に16msの倍数になる???? 精度悪すぎでは？
	*/
	std::cout << "----------------------------" << std::endl;
	for (size_t i = 0; i < 20; i++)
	{
		sw.Start("--WaitableWait");
		WaitableWait(std::chrono::milliseconds(1));
		sw.Print();
	}


	/*
	WaitableTimerを用いたウェイト
	timeBeginPeriodとの併用で最大3ms程度にはなったが今度は1ms未満が出てきた
	*/
	std::cout << "----------------------------" << std::endl;
	timeBeginPeriod(1);
	for (size_t i = 0; i < 20; i++)
	{
		sw.Start("--WaitableWait");
		WaitableWait(std::chrono::milliseconds(1));
		sw.Print();
	}
	timeEndPeriod(1);

	/*
	WaitableTImerを用いた非同期タスク実行
	基本的に16msの倍数になる???? 精度悪すぎでは？
	*/
	std::cout << "----------------------------" << std::endl;
	Waitable w;
	for (size_t i = 0; i < 5; i++)
    {
	    StopWatchG::Start("--Waitable");
	    w.PostTask(std::chrono::milliseconds(1), [&] {
			StopWatchG::Wrap("do task   ");
		});
	    std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	



	return 0;
}
