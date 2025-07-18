# io_context サンプル集

このフォルダには`boost::asio::io_context`の使い方を学ぶためのサンプルコードが含まれています。

## サンプル一覧

### 1. basic_example.cpp
**io_contextの基本的な使用方法**
- `io_context`の作成と基本的な操作
- `work_guard`の使用方法
- `post()`を使った非同期タスクの投入
- `run_one()`を使った段階的なタスク実行

**学習ポイント:**
- io_contextが何をするものかの基本理解
- work_guardの役割と重要性
- postとrun_oneの関係

### 2. timer_example.cpp
**タイマーを使用した非同期処理**
- `steady_timer`の基本的な使用方法
- 複数のタイマーの並行実行
- 繰り返しタイマーの実装
- エラーハンドリング

**学習ポイント:**
- 時間ベースの非同期処理
- タイマーのキャンセルとエラー処理
- ラムダ関数とキャプチャの活用

### 3. async_example.cpp
**マルチスレッドでの非同期処理**
- 複数スレッドでのio_context実行
- `shared_ptr`と`enable_shared_from_this`の使用
- 非同期タスクの並列実行
- スレッドセーフな処理

**学習ポイント:**
- io_contextのマルチスレッド実行モデル
- オブジェクトの生存期間管理
- 並列処理とスレッドの協調

## ビルド方法

各サンプルは個別にビルドできます：

```bash
# プロジェクトの設定
cmake -B out/build/x64-Debug -S .

# 全サンプルのビルド
cmake --build out/build/x64-Debug

# 実行例
./out/build/x64-Debug/basic_example.exe
./out/build/x64-Debug/timer_example.exe
./out/build/x64-Debug/async_example.exe
```

## io_contextの重要概念

### io_context
- 非同期I/O操作のコアエンジン
- イベントループの実装
- 複数スレッドから安全に使用可能

### work_guard
- io_contextが作業待ちで終了するのを防ぐ
- 明示的に解除するまでrun()が継続

### post() vs dispatch()
- `post()`: 必ず非同期で実行
- `dispatch()`: 現在のスレッドがio_contextを実行中なら同期実行

### run() vs run_one()
- `run()`: 全ての作業が完了するまで実行
- `run_one()`: 1つの作業のみ実行