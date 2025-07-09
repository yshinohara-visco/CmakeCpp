#include "async_data_async.h"
#include <chrono>
#include <thread>
#include <iostream>

// コンストラクタ
AsyncDataAsync::AsyncDataAsync(int initial_value, const std::string& initial_text)
    : current_data_(std::make_shared<Data>(initial_value, initial_text))
    , should_stop_(false)
    , is_processing_(false)
    , pending_tasks_(0)
    , completed_tasks_(0)
    , processing_delay_ms_(100)  // デフォルト100ms遅延
{
    // ワーカースレッドを開始
    worker_thread_ = std::thread(&AsyncDataAsync::WorkerThreadLoop, this);
}

// デストラクタ
AsyncDataAsync::~AsyncDataAsync() {
    // ワーカースレッドに停止を指示
    should_stop_.store(true);
    
    // キューの条件変数を通知してワーカースレッドを起こす
    queue_cv_.notify_all();
    
    // ワーカースレッドの終了を待機
    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }
}

// 値の更新（非同期、即座に戻る）
void AsyncDataAsync::UpdateValue(int new_value) {
    EnqueueTask(UpdateType::Value, new_value, "");
}

void AsyncDataAsync::UpdateText(const std::string& new_text) {
    EnqueueTask(UpdateType::Text, 0, new_text);
}

void AsyncDataAsync::UpdateBoth(int new_value, const std::string& new_text) {
    EnqueueTask(UpdateType::Both, new_value, new_text);
}

// 値の更新（完了まで待機可能）
std::future<void> AsyncDataAsync::UpdateValueAsync(int new_value) {
    return EnqueueTaskWithFuture(UpdateType::Value, new_value, "");
}

std::future<void> AsyncDataAsync::UpdateTextAsync(const std::string& new_text) {
    return EnqueueTaskWithFuture(UpdateType::Text, 0, new_text);
}

std::future<void> AsyncDataAsync::UpdateBothAsync(int new_value, const std::string& new_text) {
    return EnqueueTaskWithFuture(UpdateType::Both, new_value, new_text);
}

// データの読み取り
std::pair<int, std::string> AsyncDataAsync::Read(ReadBehavior behavior) {
    auto data = GetDataForRead(behavior);
    return std::make_pair(data->value, data->text);
}

int AsyncDataAsync::ReadValue(ReadBehavior behavior) {
    auto data = GetDataForRead(behavior);
    return data->value;
}

std::string AsyncDataAsync::ReadText(ReadBehavior behavior) {
    auto data = GetDataForRead(behavior);
    return data->text;
}

// 状態確認
bool AsyncDataAsync::IsProcessing() const {
    return is_processing_.load();
}

size_t AsyncDataAsync::GetPendingTaskCount() const {
    return pending_tasks_.load();
}

size_t AsyncDataAsync::GetCompletedTaskCount() const {
    return completed_tasks_.load();
}

// 処理時間シミュレーション設定
void AsyncDataAsync::SetProcessingDelay(int delay_ms) {
    processing_delay_ms_.store(delay_ms);
}

int AsyncDataAsync::GetProcessingDelay() const {
    return processing_delay_ms_.load();
}

// 全てのタスクの完了を待機
void AsyncDataAsync::WaitForAllTasks() {
    // 待機条件: キューが空で、処理中でない
    std::unique_lock<std::mutex> lock(queue_mutex_);
    queue_cv_.wait(lock, [this] { 
        return task_queue_.empty() && !is_processing_.load(); 
    });
}

// ワーカースレッドの処理ループ
void AsyncDataAsync::WorkerThreadLoop() {
    while (!should_stop_.load()) {
        std::unique_ptr<UpdateTask> task;
        
        // キューからタスクを取得
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            queue_cv_.wait(lock, [this] { 
                return !task_queue_.empty() || should_stop_.load(); 
            });
            
            if (should_stop_.load()) {
                break;
            }
            
            if (!task_queue_.empty()) {
                task = std::move(task_queue_.front());
                task_queue_.pop();
                pending_tasks_--;
            }
        }
        
        // タスクを処理
        if (task) {
            is_processing_.store(true);
            try {
                ProcessTask(*task);
                task->completion_promise.set_value();
                completed_tasks_++;
            } catch (...) {
                task->completion_promise.set_exception(std::current_exception());
            }
            is_processing_.store(false);
            
            // 待機中のスレッドに通知
            queue_cv_.notify_all();
        }
    }
}

// タスクの実行
void AsyncDataAsync::ProcessTask(UpdateTask& task) {
    // 複雑な処理のシミュレーション
    SimulateComplexProcessing();
    
    // 現在のデータのコピーを作成
    std::shared_ptr<Data> current;
    {
        std::lock_guard<std::mutex> data_lock(data_mutex_);
        current = current_data_;
    }
    auto new_data = std::make_shared<Data>(*current);
    
    // タスクの種類に応じて更新
    switch (task.type) {
        case UpdateType::Value:
            new_data->value = task.value;
            break;
        case UpdateType::Text:
            new_data->text = task.text;
            break;
        case UpdateType::Both:
            new_data->value = task.value;
            new_data->text = task.text;
            break;
    }
    
    // 新しいデータに切り替え
    {
        std::lock_guard<std::mutex> data_lock(data_mutex_);
        current_data_ = new_data;
    }
}

// 複雑な処理のシミュレーション
void AsyncDataAsync::SimulateComplexProcessing() {
    int delay_ms = processing_delay_ms_.load();
    if (delay_ms > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    
    // 追加で複雑な計算をシミュレーション
    volatile int dummy = 0;
    for (int i = 0; i < 10000; ++i) {
        dummy += i * i;
    }
}

// 読み取り処理のヘルパー関数
std::shared_ptr<AsyncDataAsync::Data> AsyncDataAsync::GetDataForRead(ReadBehavior behavior) {
    if (behavior == ReadBehavior::ReadCurrent) {
        // 現在の値を即座に取得
        std::lock_guard<std::mutex> lock(data_mutex_);
        return current_data_;
    } else {
        // 更新完了まで待機
        WaitForAllTasks();
        std::lock_guard<std::mutex> lock(data_mutex_);
        return current_data_;
    }
}

// タスクをキューに追加
template<typename... Args>
void AsyncDataAsync::EnqueueTask(Args&&... args) {
    auto task = std::make_unique<UpdateTask>(std::forward<Args>(args)...);
    
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        task_queue_.push(std::move(task));
        pending_tasks_++;
    }
    
    queue_cv_.notify_one();
}

// タスクをキューに追加（future付き）
template<typename... Args>
std::future<void> AsyncDataAsync::EnqueueTaskWithFuture(Args&&... args) {
    auto task = std::make_unique<UpdateTask>(std::forward<Args>(args)...);
    auto future = task->completion_promise.get_future();
    
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        task_queue_.push(std::move(task));
        pending_tasks_++;
    }
    
    queue_cv_.notify_one();
    return future;
}

// テンプレートの明示的インスタンス化は不要
// C++のテンプレートは使用時に自動的にインスタンス化される