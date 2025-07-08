#include <iostream>
#include <thread>
#include <chrono>
#include "Timer.h"

void SimulateWork(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main()
{
    std::cout << "=== Timer Library Demo ===" << std::endl;
    
    // 基本的な使用例
    {
        std::cout << "\n[基本的な使用例]" << std::endl;
        TimerLib::Timer timer("基本計測");
        
        timer.Start();
        SimulateWork(100);
        timer.Wrap("初期化完了");
        
        SimulateWork(200);
        timer.Wrap("データ処理完了");
        
        SimulateWork(150);
        timer.Stop();
    }
    
    // 遅延出力の例
    {
        std::cout << "\n[遅延出力の例]" << std::endl;
        TimerLib::Timer timer("遅延計測");
        timer.SetDelayedOutput(true);
        
        timer.Start();
        SimulateWork(80);
        timer.Wrap("クリティカル処理1");
        
        SimulateWork(120);
        timer.Wrap("クリティカル処理2");
        
        SimulateWork(100);
        timer.Stop();
        
        std::cout << "クリティカル処理完了（出力は遅延されました）" << std::endl;
        std::cout << "結果を出力します:" << std::endl;
        timer.FlushOutput();
    }
    
    // 複数のタイマーの管理例
    {
        std::cout << "\n[複数タイマーの管理例]" << std::endl;
        auto timer1 = std::make_shared<TimerLib::Timer>("タイマー1");
        auto timer2 = std::make_shared<TimerLib::Timer>("タイマー2");
        
        // タイマーマネージャーに登録
        TimerLib::TimerManager::Instance().RegisterTimer(timer1);
        TimerLib::TimerManager::Instance().RegisterTimer(timer2);
        TimerLib::TimerManager::Instance().SetGlobalDelayedOutput(true);
        
        timer1->Start();
        SimulateWork(50);
        timer1->Wrap("タスクA");
        
        timer2->Start();
        SimulateWork(75);
        timer2->Wrap("タスクB");
        
        SimulateWork(60);
        timer1->Stop();
        timer2->Stop();
        
        std::cout << "全タイマーの結果を一括出力:" << std::endl;
        TimerLib::TimerManager::Instance().FlushAllTimers();
    }
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}
