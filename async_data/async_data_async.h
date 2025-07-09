#pragma once
#include <atomic>
#include <memory>
#include <string>
#include <utility>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <future>
#include "common.h"

// 非同期でアクセス可能なデータクラス（非同期更新版）
class AsyncDataAsync {
private:
    // データ構造体
    struct Data {
        int value;
        std::string text;
        
        Data(int v, const std::string& t) : value(v), text(t) {}
        Data(const Data& other) : value(other.value), text(other.text) {}
    };
    
    // 更新タスク構造体
    struct UpdateTask {
        UpdateType type;
        int value;
        std::string text;
        std::promise<void> completion_promise;  // 完了通知用
        
        UpdateTask(UpdateType t, int v, const std::string& txt) 
            : type(t), value(v), text(txt) {}
    };
    
    // 現在のアクティブなデータ
    std::shared_ptr<Data> current_data_;
    mutable std::mutex data_mutex_;  // データ読み取り用
    
    // 更新タスクキュー
    std::queue<std::unique_ptr<UpdateTask>> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    
    // ワーカースレッド管理
    std::thread worker_thread_;
    std::atomic<bool> should_stop_;
    std::atomic<bool> is_processing_;
    
    // 統計情報
    std::atomic<size_t> pending_tasks_;
    std::atomic<size_t> completed_tasks_;
    
    // 処理時間のシミュレーション設定
    std::atomic<int> processing_delay_ms_;
    
public:
    // コンストラクタ
    explicit AsyncDataAsync(int initial_value = 0, const std::string& initial_text = "");
    
    // デストラクタ
    ~AsyncDataAsync();
    
    // コピー・ムーブコンストラクタ/代入演算子は削除
    AsyncDataAsync(const AsyncDataAsync&) = delete;
    AsyncDataAsync& operator=(const AsyncDataAsync&) = delete;
    AsyncDataAsync(AsyncDataAsync&&) = delete;
    AsyncDataAsync& operator=(AsyncDataAsync&&) = delete;
    
    // 値の更新（非同期、即座に戻る）
    void UpdateValue(int new_value);
    void UpdateText(const std::string& new_text);
    void UpdateBoth(int new_value, const std::string& new_text);
    
    // 値の更新（完了まで待機可能）
    std::future<void> UpdateValueAsync(int new_value);
    std::future<void> UpdateTextAsync(const std::string& new_text);
    std::future<void> UpdateBothAsync(int new_value, const std::string& new_text);
    
    // データの読み取り
    std::pair<int, std::string> Read(ReadBehavior behavior = ReadBehavior::ReadCurrent);
    int ReadValue(ReadBehavior behavior = ReadBehavior::ReadCurrent);
    std::string ReadText(ReadBehavior behavior = ReadBehavior::ReadCurrent);
    
    // 状態確認
    bool IsProcessing() const;
    size_t GetPendingTaskCount() const;
    size_t GetCompletedTaskCount() const;
    
    // 処理時間シミュレーション設定
    void SetProcessingDelay(int delay_ms);
    int GetProcessingDelay() const;
    
    // 全てのタスクの完了を待機
    void WaitForAllTasks();
    
private:
    // ワーカースレッドの処理ループ
    void WorkerThreadLoop();
    
    // タスクの実行
    void ProcessTask(UpdateTask& task);
    
    // 複雑な処理のシミュレーション
    void SimulateComplexProcessing();
    
    // 読み取り処理のヘルパー関数
    std::shared_ptr<Data> GetDataForRead(ReadBehavior behavior);
    
    // タスクをキューに追加
    template<typename... Args>
    void EnqueueTask(Args&&... args);
    
    // タスクをキューに追加（future付き）
    template<typename... Args>
    std::future<void> EnqueueTaskWithFuture(Args&&... args);
};