#include "async_data.h"
#include <functional>

// コンストラクタ
AsyncData::AsyncData(int initial_value, const std::string& initial_text) 
    : current_data_(std::make_shared<Data>(initial_value, initial_text))
    , update_in_progress_(false) {
}

// 値の更新
void AsyncData::UpdateValue(int new_value) {
    PerformUpdate([new_value](Data& data) {
        data.value = new_value;
    });
}

// テキストの更新
void AsyncData::UpdateText(const std::string& new_text) {
    PerformUpdate([&new_text](Data& data) {
        data.text = new_text;
    });
}

// 両方の更新
void AsyncData::UpdateBoth(int new_value, const std::string& new_text) {
    PerformUpdate([new_value, &new_text](Data& data) {
        data.value = new_value;
        data.text = new_text;
    });
}

// データの読み取り
std::pair<int, std::string> AsyncData::Read(ReadBehavior behavior) {
    auto data = GetDataForRead(behavior);
    return std::make_pair(data->value, data->text);
}

// 値の読み取り
int AsyncData::ReadValue(ReadBehavior behavior) {
    auto data = GetDataForRead(behavior);
    return data->value;
}

// テキストの読み取り
std::string AsyncData::ReadText(ReadBehavior behavior) {
    auto data = GetDataForRead(behavior);
    return data->text;
}

// 更新中かどうかを確認
bool AsyncData::IsUpdating() const {
    return update_in_progress_.load();
}

// 更新処理のヘルパー関数
void AsyncData::PerformUpdate(std::function<void(Data&)> update_func) {
    std::unique_lock<std::mutex> lock(update_mutex_);
    
    // 更新開始フラグを設定
    update_in_progress_.store(true);
    
    // 現在のデータのコピーを作成
    auto current = current_data_;
    auto new_data = std::make_shared<Data>(*current);
    
    // 更新関数を適用
    update_func(*new_data);
    
    // 新しいデータに切り替え
    current_data_ = new_data;
    
    // 更新完了フラグを設定
    update_in_progress_.store(false);
    
    // 待機中のスレッドに通知
    update_cv_.notify_all();
}

// 読み取り処理のヘルパー関数
std::shared_ptr<AsyncData::Data> AsyncData::GetDataForRead(ReadBehavior behavior) {
    if (behavior == ReadBehavior::ReadCurrent) {
        // 現在の値を即座に取得
        std::unique_lock<std::mutex> lock(update_mutex_);
        return current_data_;
    } else {
        // 更新完了まで待機
        std::unique_lock<std::mutex> lock(update_mutex_);
        update_cv_.wait(lock, [this] { return !update_in_progress_.load(); });
        return current_data_;
    }
}