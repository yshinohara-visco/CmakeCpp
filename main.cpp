#include <iostream>
#include <thread>
#include <chrono>
#include "math_operations.h"
#include "async_data.h"
#include "async_data_async.h"

int main()
{
    std::cout << "=== Math Operations Library Demo ===" << std::endl;
    
    // Math Operations Library Test
    {
        std::cout << "\n[Math Operations Library Test]" << std::endl;
        
        int a = 10, b = 5;
        std::cout << "a = " << a << ", b = " << b << std::endl;
        
        std::cout << "Addition: " << a << " + " << b << " = " << MathOperations::add(a, b) << std::endl;
        std::cout << "Subtraction: " << a << " - " << b << " = " << MathOperations::subtract(a, b) << std::endl;
        std::cout << "Multiplication: " << a << " * " << b << " = " << MathOperations::multiply(a, b) << std::endl;
        std::cout << "Division: " << a << " / " << b << " = " << MathOperations::divide(a, b) << std::endl;
        
        // Test division by zero exception
        try {
            std::cout << "Division by zero test: " << a << " / 0 = ";
            MathOperations::divide(a, 0);
        } catch (const std::exception& e) {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }
    }
    
    // Async Data Library Test
    {
        std::cout << "\n[Async Data Library Test]" << std::endl;
        
        AsyncData async_data(42, "initial");
        
        // 基本的な読み取りテスト
        auto [value, text] = async_data.Read();
        std::cout << "初期値: value=" << value << ", text=\"" << text << "\"" << std::endl;
        
        // 更新テスト
        async_data.UpdateValue(100);
        async_data.UpdateText("updated");
        
        // 更新後の読み取り
        auto [new_value, new_text] = async_data.Read();
        std::cout << "更新後: value=" << new_value << ", text=\"" << new_text << "\"" << std::endl;
        
        // 非同期更新テスト
        std::cout << "\n非同期更新テスト..." << std::endl;
        
        std::thread updater([&async_data]() {
            for (int i = 0; i < 5; ++i) {
                async_data.UpdateValue(i * 10);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
        
        std::thread reader([&async_data]() {
            for (int i = 0; i < 10; ++i) {
                int current_value = async_data.ReadValue(ReadBehavior::ReadCurrent);
                std::cout << "読み取り値: " << current_value << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });
        
        updater.join();
        reader.join();
        
        std::cout << "非同期テスト完了" << std::endl;
    }
    
    // Async Data Library (Async Version) Test
    {
        std::cout << "\n[Async Data Library (Async Version) Test]" << std::endl;
        
        AsyncDataAsync async_data_async(0, "initial");
        
        // 処理遅延を設定（複雑な処理をシミュレート）
        async_data_async.SetProcessingDelay(200);
        
        // 基本的な読み取りテスト
        auto [value, text] = async_data_async.Read();
        std::cout << "初期値: value=" << value << ", text=\"" << text << "\"" << std::endl;
        
        // 即座返却の確認
        std::cout << "\n即座返却テスト..." << std::endl;
        auto start_time = std::chrono::high_resolution_clock::now();
        
        async_data_async.UpdateValue(100);
        async_data_async.UpdateText("processing");
        async_data_async.UpdateBoth(200, "final");
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        std::cout << "3つの更新要求の送信時間: " << duration.count() << "ms" << std::endl;
        std::cout << "キューイング中のタスク数: " << async_data_async.GetPendingTaskCount() << std::endl;
        std::cout << "処理中状態: " << (async_data_async.IsProcessing() ? "Yes" : "No") << std::endl;
        
        // 非同期処理の進行状況を監視
        std::cout << "\n処理進行状況を監視..." << std::endl;
        while (async_data_async.GetPendingTaskCount() > 0 || async_data_async.IsProcessing()) {
            std::cout << "  残りタスク: " << async_data_async.GetPendingTaskCount() 
                      << ", 完了タスク: " << async_data_async.GetCompletedTaskCount()
                      << ", 処理中: " << (async_data_async.IsProcessing() ? "Yes" : "No") << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // 最終結果を確認
        auto [final_value, final_text] = async_data_async.Read();
        std::cout << "最終値: value=" << final_value << ", text=\"" << final_text << "\"" << std::endl;
        std::cout << "完了タスク数: " << async_data_async.GetCompletedTaskCount() << std::endl;
        
        // Future使用例
        std::cout << "\nFuture使用例..." << std::endl;
        auto future1 = async_data_async.UpdateValueAsync(500);
        auto future2 = async_data_async.UpdateTextAsync("future_test");
        
        std::cout << "更新要求送信完了、待機中..." << std::endl;
        future1.wait();
        future2.wait();
        std::cout << "Future完了" << std::endl;
        
        auto [future_value, future_text] = async_data_async.Read();
        std::cout << "Future結果: value=" << future_value << ", text=\"" << future_text << "\"" << std::endl;
        
        // 全タスクの完了を待機
        async_data_async.WaitForAllTasks();
        std::cout << "全タスク完了" << std::endl;
    }
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}
