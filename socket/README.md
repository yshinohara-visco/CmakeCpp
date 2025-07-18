# Socket通信サンプル

このディレクトリには、Boost.Asioを使用した複数のTCP通信プロトコルの実装が含まれています。

## 概要

2つの異なるメッセージプロトコルと、それらの共通基盤となるアーキテクチャを提供します：

1. **区切り文字プロトコル** - 改行文字等でメッセージを区切る方式
2. **固定長ヘッダープロトコル** - ID+サイズのヘッダーでメッセージ境界を明確化
3. **エコーサーバー** - 動作確認済みのシンプルな実装

## ファイル構成

### 共通基盤
- `connection_base.hpp/cpp` - 全プロトコル共通の基底クラス
- `CMakeLists.txt` - ビルド設定

### 区切り文字プロトコル
- `delimited_connection.hpp/cpp` - 区切り文字による通信実装
- `delimited_server.cpp` - サーバー（ポート8081）
- `delimited_client.cpp` - テスト用クライアント

### 固定長ヘッダープロトコル
- `header_connection.hpp/cpp` - ヘッダー付き通信実装
- `header_server.cpp` - サーバー（ポート8082）
- `header_client.cpp` - テスト用クライアント

### レガシーエコーサーバー
- `echo_server.cpp` - シンプルなエコーサーバー（ポート8080）
- `echo_client.cpp` - エコークライアント

## ビルド方法

```bash
# プロジェクトルートから
cmake -B out/build/claude -S . -G "Visual Studio 17 2022" -A x64
cmake --build out/build/claude --config Debug
```

## 実行方法

### 1. 区切り文字プロトコルのテスト

**サーバー起動:**
```bash
./out/build/claude/socket/Debug/delimited_server.exe
```

**クライアント実行:**
```bash
./out/build/claude/socket/Debug/delimited_client.exe
```

**手動テスト（telnet）:**
```bash
telnet localhost 8081
```

### 2. 固定長ヘッダープロトコルのテスト

**サーバー起動:**
```bash
./out/build/claude/socket/Debug/header_server.exe
```

**クライアント実行:**
```bash
./out/build/claude/socket/Debug/header_client.exe
```

### 3. エコーサーバーのテスト

**サーバー起動:**
```bash
./out/build/claude/socket/Debug/echo_server.exe
```

**クライアント実行:**
```bash
./out/build/claude/socket/Debug/echo_client.exe
```

## プロトコル仕様

### 区切り文字プロトコル

**特徴:**
- 改行文字（`\n`）でメッセージを区切り
- テキストベースの通信に適している
- シンプルで実装が容易

**メッセージ形式:**
```
メッセージ内容\n
```

**使用例:**
- チャットアプリケーション
- コマンドラインツール
- テキストベースのAPI

**メリット:**
- 人間が読みやすい
- デバッグが容易
- telnetでテスト可能

**デメリット:**
- バイナリデータに不向き
- メッセージ内に区切り文字が含まれる場合の処理が必要

### 固定長ヘッダープロトコル

**特徴:**
- 8バイトのヘッダー（ID4バイト + サイズ4バイト）
- バイナリデータ対応
- メッセージ境界が明確

**メッセージ形式:**
```
[4バイトID][4バイトサイズ][メッセージ本体]
```

**バイトオーダー:**
- ネットワークバイトオーダー（ビッグエンディアン）を使用
- `htonl()`/`ntohl()`で変換

**使用例:**
- ゲームサーバー
- ファイル転送
- バイナリAPI

**メリット:**
- バイナリデータ対応
- メッセージサイズが事前に分かる
- 効率的な受信処理

**デメリット:**
- 実装が複雑
- デバッグが困難
- ヘッダーのオーバーヘッド

## アーキテクチャ

### ConnectionBase（基底クラス）

**責務:**
- ソケット管理
- 非同期送受信の共通処理
- エラーハンドリング
- コールバック管理

**主要メソッド:**
```cpp
void start()                           // 接続開始
void send_message(const std::string&)  // メッセージ送信
void disconnect()                      // 接続終了
void set_message_handler(handler)      // 受信時コールバック
void set_error_handler(handler)        // エラー時コールバック
```

### ServerBase（サーバー基底クラス）

**責務:**
- 接続受付
- 新しい接続の管理
- テンプレートによる型安全性

**使用方法:**
```cpp
class MyServer : public ServerBase<MyConnection> {
protected:
    void on_new_connection(std::shared_ptr<MyConnection> conn) override {
        // 新しい接続の処理
    }
};
```

### 継承構造

```
ConnectionBase
├── DelimitedConnection  (区切り文字)
└── HeaderConnection     (固定長ヘッダー)

ServerBase<T>
├── DelimitedServer
└── HeaderServer
```

## 設計原則

### 1. 共通化
- 接続管理、エラーハンドリング等の共通処理を基底クラスに集約
- 重複コードの排除

### 2. 拡張性
- 新しいプロトコルは`ConnectionBase`を継承するだけで実装可能
- プロトコル固有の処理のみを実装

### 3. 安全性
- `std::enable_shared_from_this`による適切なオブジェクト生存期間管理
- 非同期処理でのメモリ安全性を保証

### 4. 実用性
- 実際のアプリケーションで使用可能な堅牢な実装
- 適切なエラーハンドリングとロギング

## 使用技術

- **C++23** - モダンC++の機能を活用
- **Boost.Asio** - 非同期I/O処理
- **UTF-8 with BOM** - 日本語対応のファイルエンコーディング
- **CMake** - クロスプラットフォームビルド
- **Visual Studio 2022** - 開発環境

## 今後の拡張

### 新しいプロトコルの追加例

1. **JSONプロトコル** - JSON形式でのメッセージ交換
2. **圧縮プロトコル** - gzip等での圧縮通信
3. **暗号化プロトコル** - TLS/SSL対応
4. **HTTP風プロトコル** - HTTPライクなヘッダー形式

### 機能拡張

1. **認証機能** - ユーザー認証機能
2. **ルーティング** - メッセージタイプ別の処理振り分け
3. **ロードバランシング** - 複数サーバーへの負荷分散
4. **監視機能** - 接続数、スループット等の監視

## 参考情報

- [Boost.Asio Documentation](https://www.boost.org/doc/libs/1_82_0/doc/html/boost_asio.html)
- [TCP/IPプロトコル解説](https://tools.ietf.org/html/rfc793)
- [ネットワークバイトオーダー](https://tools.ietf.org/html/rfc1700)