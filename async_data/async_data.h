#pragma once
#include <atomic>
#include <memory>
#include <string>
#include <utility>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "common.h"

// 非同期でアクセス可能なデータクラス
class AsyncData {
private:
    // データ構造体
    struct Data {
        int value;
        std::string text;
        
        Data(int v, const std::string& t) : value(v), text(t) {}
        Data(const Data& other) : value(other.value), text(other.text) {}
    };
    
    // 現在のアクティブなデータ
    std::shared_ptr<Data> current_data_;
    
    // 更新用の排他制御
    std::mutex update_mutex_;
    
    // 更新完了通知用
    std::condition_variable update_cv_;
    std::atomic<bool> update_in_progress_;
    
public:
    // コンストラクタ
    explicit AsyncData(int initial_value = 0, const std::string& initial_text = "");
    
    // デストラクタ
    ~AsyncData() = default;
    
    // コピー・ムーブコンストラクタ/代入演算子は削除
    AsyncData(const AsyncData&) = delete;
    AsyncData& operator=(const AsyncData&) = delete;
    AsyncData(AsyncData&&) = delete;
    AsyncData& operator=(AsyncData&&) = delete;
    
    // 値の更新
    void UpdateValue(int new_value);
    void UpdateText(const std::string& new_text);
    void UpdateBoth(int new_value, const std::string& new_text);
    
    // データの読み取り
    std::pair<int, std::string> Read(ReadBehavior behavior = ReadBehavior::ReadCurrent);
    int ReadValue(ReadBehavior behavior = ReadBehavior::ReadCurrent);
    std::string ReadText(ReadBehavior behavior = ReadBehavior::ReadCurrent);
    
    // デバッグ用: 更新中かどうかを確認
    bool IsUpdating() const;
    
private:
    // 更新処理のヘルパー関数
    void PerformUpdate(std::function<void(Data&)> update_func);
    
    // 読み取り処理のヘルパー関数
    std::shared_ptr<Data> GetDataForRead(ReadBehavior behavior);
};