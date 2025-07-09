#pragma once

// 読み取り時の動作指定
enum class ReadBehavior {
    ReadCurrent,    // 現在の値を即座に取得
    WaitForUpdate   // 更新完了まで待機
};

// 更新タスクの種類
enum class UpdateType {
    Value,      // 値のみ更新
    Text,       // テキストのみ更新
    Both        // 両方更新
};