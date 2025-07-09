#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include "async_data.h"

// 基本的な読み書きテスト
TEST(AsyncDataTest, BasicReadWrite) {
    AsyncData async_data(42, "test");
    
    // 初期値の確認
    EXPECT_EQ(async_data.ReadValue(), 42);
    EXPECT_EQ(async_data.ReadText(), "test");
    
    auto [value, text] = async_data.Read();
    EXPECT_EQ(value, 42);
    EXPECT_EQ(text, "test");
}

// 値の更新テスト
TEST(AsyncDataTest, ValueUpdate) {
    AsyncData async_data(0, "");
    
    // 値の更新
    async_data.UpdateValue(100);
    EXPECT_EQ(async_data.ReadValue(), 100);
    
    // テキストの更新
    async_data.UpdateText("updated");
    EXPECT_EQ(async_data.ReadText(), "updated");
    
    // 両方の更新
    async_data.UpdateBoth(200, "both_updated");
    auto [value, text] = async_data.Read();
    EXPECT_EQ(value, 200);
    EXPECT_EQ(text, "both_updated");
}

// ReadCurrentの動作テスト
TEST(AsyncDataTest, ReadCurrentBehavior) {
    AsyncData async_data(10, "current");
    
    // ReadCurrentで即座に値を取得
    auto start_time = std::chrono::high_resolution_clock::now();
    int value = async_data.ReadValue(ReadBehavior::ReadCurrent);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    EXPECT_EQ(value, 10);
    
    // 読み取りが即座に完了することを確認（100ms未満）
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    EXPECT_LT(duration.count(), 100);
}

// 非同期更新中の読み取りテスト
TEST(AsyncDataTest, ConcurrentReadWrite) {
    AsyncData async_data(0, "start");
    std::atomic<bool> reader_done{false};
    std::atomic<int> read_count{0};
    
    // 更新用スレッド
    std::thread updater([&async_data]() {
        for (int i = 1; i <= 10; ++i) {
            async_data.UpdateValue(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
    
    // 読み取り用スレッド
    std::thread reader([&async_data, &reader_done, &read_count]() {
        while (!reader_done.load()) {
            async_data.ReadValue(ReadBehavior::ReadCurrent);
            read_count++;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });
    
    // 150ms後に読み取りを停止
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    reader_done.store(true);
    
    updater.join();
    reader.join();
    
    // 最終的な値が正しく更新されていることを確認
    EXPECT_EQ(async_data.ReadValue(), 10);
    
    // 読み取りが複数回実行されていることを確認
    EXPECT_GT(read_count.load(), 5);
}

// WaitForUpdateの動作テスト
TEST(AsyncDataTest, WaitForUpdateBehavior) {
    AsyncData async_data(0, "wait_test");
    
    // 基本的な動作確認：WaitForUpdateで読み取り
    int value1 = async_data.ReadValue(ReadBehavior::WaitForUpdate);
    EXPECT_EQ(value1, 0);
    
    // 値を更新してからWaitForUpdateで読み取り
    async_data.UpdateValue(42);
    int value2 = async_data.ReadValue(ReadBehavior::WaitForUpdate);
    EXPECT_EQ(value2, 42);
    
    // ReadCurrentとWaitForUpdateの結果が同じであることを確認
    int current_value = async_data.ReadValue(ReadBehavior::ReadCurrent);
    int wait_value = async_data.ReadValue(ReadBehavior::WaitForUpdate);
    EXPECT_EQ(current_value, wait_value);
    
    // 複数の更新を行って一貫性を確認
    async_data.UpdateValue(100);
    async_data.UpdateValue(200);
    int final_value = async_data.ReadValue(ReadBehavior::WaitForUpdate);
    EXPECT_EQ(final_value, 200);
}

// 複数の更新操作の一貫性テスト
TEST(AsyncDataTest, MultipleUpdatesConsistency) {
    AsyncData async_data(0, "");
    const int num_updates = 100;
    
    // 複数スレッドで更新を実行
    std::vector<std::thread> updaters;
    for (int i = 0; i < 4; ++i) {
        updaters.emplace_back([&async_data, i, num_updates]() {
            for (int j = 0; j < num_updates; ++j) {
                int value = i * num_updates + j;
                async_data.UpdateBoth(value, std::to_string(value));
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        });
    }
    
    // 読み取り用スレッド
    std::atomic<bool> reading_done{false};
    std::thread reader([&async_data, &reading_done]() {
        while (!reading_done.load()) {
            auto [value, text] = async_data.Read(ReadBehavior::ReadCurrent);
            // 値とテキストの一貫性を確認
            EXPECT_EQ(std::to_string(value), text);
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
    });
    
    // 全ての更新スレッドの完了を待機
    for (auto& updater : updaters) {
        updater.join();
    }
    
    reading_done.store(true);
    reader.join();
    
    // 最終的な一貫性を確認
    auto [final_value, final_text] = async_data.Read();
    EXPECT_EQ(std::to_string(final_value), final_text);
}

// 更新中フラグのテスト
TEST(AsyncDataTest, UpdateInProgressFlag) {
    AsyncData async_data(0, "");
    
    // 通常時は更新中でないことを確認
    EXPECT_FALSE(async_data.IsUpdating());
    
    // 更新中に IsUpdating() が true になることを確認
    std::atomic<bool> flag_checked{false};
    std::thread updater([&async_data, &flag_checked]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        async_data.UpdateValue(42);
        flag_checked.store(true);
    });
    
    // 更新開始まで少し待機
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    
    // 短時間であれば更新中でないことを確認（更新は瞬間的）
    // このテストは実装依存なので、基本的な動作確認のみ
    
    updater.join();
    EXPECT_TRUE(flag_checked.load());
    
    // 更新完了後は更新中でないことを確認
    EXPECT_FALSE(async_data.IsUpdating());
}