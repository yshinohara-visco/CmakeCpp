#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <future>
#include "async_data_async.h"

// 基本的な読み書きテスト
TEST(AsyncDataAsyncTest, BasicReadWrite) {
    AsyncDataAsync async_data(42, "test");
    
    // 初期値の確認
    EXPECT_EQ(async_data.ReadValue(), 42);
    EXPECT_EQ(async_data.ReadText(), "test");
    
    auto [value, text] = async_data.Read();
    EXPECT_EQ(value, 42);
    EXPECT_EQ(text, "test");
    
    // 全タスクの完了を待機
    async_data.WaitForAllTasks();
}

// 非同期更新の即座返却テスト
TEST(AsyncDataAsyncTest, ImmediateReturn) {
    AsyncDataAsync async_data(0, "");
    
    // 処理遅延を設定
    async_data.SetProcessingDelay(200);
    
    // 更新の実行時間を測定
    auto start_time = std::chrono::high_resolution_clock::now();
    async_data.UpdateValue(42);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    // 即座に戻ることを確認（100ms未満）
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    EXPECT_LT(duration.count(), 100);
    
    // タスクがキューに追加されていることを確認
    EXPECT_GT(async_data.GetPendingTaskCount(), 0);
    
    // 処理完了を待機
    async_data.WaitForAllTasks();
    
    // 値が正しく更新されていることを確認
    EXPECT_EQ(async_data.ReadValue(), 42);
}

// 複数タスクのキューイングテスト
TEST(AsyncDataAsyncTest, TaskQueueing) {
    AsyncDataAsync async_data(0, "");
    
    // 処理遅延を設定
    async_data.SetProcessingDelay(50);
    
    // 複数の更新タスクを連続で送信
    for (int i = 1; i <= 5; ++i) {
        async_data.UpdateValue(i * 10);
    }
    
    // 複数のタスクがキューに追加されていることを確認
    EXPECT_GE(async_data.GetPendingTaskCount(), 1);
    
    // 全てのタスクが完了するまで待機
    async_data.WaitForAllTasks();
    
    // 最後の値が設定されていることを確認
    EXPECT_EQ(async_data.ReadValue(), 50);
    
    // 全てのタスクが完了していることを確認
    EXPECT_EQ(async_data.GetPendingTaskCount(), 0);
    EXPECT_EQ(async_data.GetCompletedTaskCount(), 5);
}

// Future による完了待機テスト
TEST(AsyncDataAsyncTest, FutureCompletion) {
    AsyncDataAsync async_data(0, "");
    
    // 処理遅延を設定
    async_data.SetProcessingDelay(100);
    
    // 非同期更新を実行
    auto future = async_data.UpdateValueAsync(123);
    
    // 完了を待機
    EXPECT_EQ(future.wait_for(std::chrono::milliseconds(50)), std::future_status::timeout);
    EXPECT_EQ(future.wait_for(std::chrono::milliseconds(200)), std::future_status::ready);
    
    // 値が正しく更新されていることを確認
    EXPECT_EQ(async_data.ReadValue(), 123);
}

// 処理遅延設定テスト
TEST(AsyncDataAsyncTest, ProcessingDelay) {
    AsyncDataAsync async_data(0, "");
    
    // 遅延時間を設定
    async_data.SetProcessingDelay(150);
    EXPECT_EQ(async_data.GetProcessingDelay(), 150);
    
    // 更新の実行時間を測定
    auto start_time = std::chrono::high_resolution_clock::now();
    auto future = async_data.UpdateValueAsync(42);
    future.wait();
    auto end_time = std::chrono::high_resolution_clock::now();
    
    // 遅延時間を含めた処理時間を確認
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    EXPECT_GE(duration.count(), 100);  // 最低100ms以上
    EXPECT_LT(duration.count(), 300);  // 300ms未満
}

// 統計情報テスト
TEST(AsyncDataAsyncTest, Statistics) {
    AsyncDataAsync async_data(0, "");
    
    // 処理遅延を設定
    async_data.SetProcessingDelay(30);
    
    // 初期状態の確認
    EXPECT_EQ(async_data.GetPendingTaskCount(), 0);
    EXPECT_EQ(async_data.GetCompletedTaskCount(), 0);
    EXPECT_FALSE(async_data.IsProcessing());
    
    // 複数のタスクを送信
    const int task_count = 3;
    for (int i = 0; i < task_count; ++i) {
        async_data.UpdateValue(i);
    }
    
    // 処理中の状態を確認
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_TRUE(async_data.IsProcessing() || async_data.GetPendingTaskCount() > 0);
    
    // 全てのタスクが完了するまで待機
    async_data.WaitForAllTasks();
    
    // 最終状態の確認
    EXPECT_EQ(async_data.GetPendingTaskCount(), 0);
    EXPECT_EQ(async_data.GetCompletedTaskCount(), task_count);
    EXPECT_FALSE(async_data.IsProcessing());
}

// 並行読み書きテスト
TEST(AsyncDataAsyncTest, ConcurrentReadWrite) {
    AsyncDataAsync async_data(0, "start");
    
    // 処理遅延を設定
    async_data.SetProcessingDelay(20);
    
    std::atomic<int> read_count{0};
    std::atomic<bool> reader_done{false};
    
    // 更新用スレッド
    std::thread updater([&async_data]() {
        for (int i = 1; i <= 10; ++i) {
            async_data.UpdateBoth(i, std::to_string(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
    
    // 読み取り用スレッド
    std::thread reader([&async_data, &read_count, &reader_done]() {
        while (!reader_done.load()) {
            auto [value, text] = async_data.Read(ReadBehavior::ReadCurrent);
            read_count++;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });
    
    // 更新スレッドの完了を待機
    updater.join();
    
    // 全てのタスクが完了するまで待機
    async_data.WaitForAllTasks();
    
    // 読み取りスレッドを停止
    reader_done.store(true);
    reader.join();
    
    // 最終的な値が正しく設定されていることを確認
    auto [final_value, final_text] = async_data.Read();
    EXPECT_EQ(final_value, 10);
    EXPECT_EQ(final_text, "10");
    
    // 複数回の読み取りが実行されていることを確認
    EXPECT_GT(read_count.load(), 10);
}

// ReadBehavior::WaitForUpdate テスト
TEST(AsyncDataAsyncTest, WaitForUpdateBehavior) {
    AsyncDataAsync async_data(0, "");
    
    // 処理遅延を設定
    async_data.SetProcessingDelay(100);
    
    // 更新タスクを送信
    async_data.UpdateValue(42);
    
    // 更新完了まで待機して読み取り
    auto start_time = std::chrono::high_resolution_clock::now();
    int value = async_data.ReadValue(ReadBehavior::WaitForUpdate);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    // 値が正しく更新されていることを確認
    EXPECT_EQ(value, 42);
    
    // 適切な時間待機していることを確認
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    EXPECT_GE(duration.count(), 80);  // 最低80ms以上
}

// 複数の更新タイプのテスト
TEST(AsyncDataAsyncTest, MultipleUpdateTypes) {
    AsyncDataAsync async_data(0, "");
    
    // 処理遅延を短く設定
    async_data.SetProcessingDelay(10);
    
    // 初期状態でのタスク数を記録
    size_t initial_completed = async_data.GetCompletedTaskCount();
    
    // 異なる種類の更新を実行
    auto future1 = async_data.UpdateValueAsync(100);
    auto future2 = async_data.UpdateTextAsync("test");
    auto future3 = async_data.UpdateBothAsync(200, "both");
    
    // 全ての更新が完了するまで待機
    future1.wait();
    future2.wait();
    future3.wait();
    
    // 最終的な値を確認（最後の更新が適用されている）
    auto [value, text] = async_data.Read();
    EXPECT_EQ(value, 200);
    EXPECT_EQ(text, "both");
    
    // 3つのタスクが追加で完了していることを確認
    EXPECT_EQ(async_data.GetCompletedTaskCount(), initial_completed + 3);
}

// 高負荷テスト
TEST(AsyncDataAsyncTest, HighLoad) {
    AsyncDataAsync async_data(0, "");
    
    // 処理遅延を短く設定
    async_data.SetProcessingDelay(1);
    
    const int task_count = 100;
    std::vector<std::future<void>> futures;
    
    // 大量のタスクを送信
    for (int i = 0; i < task_count; ++i) {
        futures.push_back(async_data.UpdateValueAsync(i));
    }
    
    // 全てのタスクの完了を待機
    for (auto& future : futures) {
        future.wait();
    }
    
    // 最終的な値を確認
    EXPECT_EQ(async_data.ReadValue(), task_count - 1);
    
    // 全てのタスクが完了していることを確認
    EXPECT_EQ(async_data.GetCompletedTaskCount(), task_count);
    EXPECT_EQ(async_data.GetPendingTaskCount(), 0);
}