# CmakeCpp

## 概要
DIが良く分からないので、適当なクラス群を作ってからそれをDI対応してもらうのはどうか。

## 予備
DIをClaudeにやってもらってみる

## プロジェクト構成

本プロジェクトでは、同じ機能を持つクラス群を2つの方法で実装しています:

- **origin/**: 従来の依存関係管理（手動でオブジェクト生成）
- **di/**: Dependency Injection（DI）を使った依存関係管理（Boost.DI使用）

## クラス構成

### クラス図
```
Engine
├── Logger (Engineが所有、他のクラスは参照を保持)
├── DataManager
│   ├── CalculationManager
│   └── StatisticsManager
└── FileManager
```

### 各クラスの役割

#### Engine
- エントリポイントとなるクラス
- Loggerを所有し、DataManagerとFileManagerを管理
- 初期化処理でセッションIDを生成し、FileManagerに渡す

#### Logger
- ログ出力を担当
- 処理中の内容をコンソールに出力
- **origin版**: Engineがunique_ptrで所有、他のクラスは参照（`Logger&`）で保持
- **DI版**: DIコンテナでシングルトンとして管理、全クラスがshared_ptrで共有

#### DataManager
- データ処理の統括クラス
- CalculationManagerとStatisticsManagerを管理
- 両方の処理結果を統合して文字列として返す

#### CalculationManager
- 計算処理を担当
- データの合計値を算出

#### StatisticsManager
- 統計処理を担当
- 受け取った値の平均を計算

#### FileManager
- ファイル出力を担当
- コンストラクタでセッションIDを受け取る
- 結果をフォーマットしてファイルに書き出す

## 初期化と処理の流れ

### origin版の初期化フロー
```
main()
  └── origin::Engine::Engine()
        ├── Logger生成 (unique_ptr)
        ├── DataManager::DataManager(logger)
        │     ├── CalculationManager::CalculationManager(logger)
        │     └── StatisticsManager::StatisticsManager(logger)
        ├── Engine::initialize()  // セッションID生成
        └── FileManager::FileManager(sessionId, logger)
```

**特徴**:
- Engineが全ての依存オブジェクトを直接生成（`std::make_unique`）
- Loggerへの参照を各クラスのコンストラクタで明示的に渡す
- 生成順序を手動で制御する必要がある

### DI版の初期化フロー
```
main()
  └── di::createModule()  // DIコンテナの設定
        ├── Logger をシングルトンとしてバインド
        └── SessionId をファクトリ関数でバインド
  └── injector.create<std::shared_ptr<di::Engine>>()
        └── DIコンテナが依存関係を自動解決
              ├── Logger生成 (singleton)
              ├── CalculationManager(logger)
              ├── StatisticsManager(logger)
              ├── DataManager(logger, calc, stats)
              ├── FileManager(sessionId, logger)
              └── Engine(logger, dataManager, fileManager)
```

**特徴**:
- DIコンテナが依存関係グラフを解析して自動生成
- 各クラスはコンストラクタで必要な依存を宣言するだけ
- 生成順序はDIコンテナが自動的に決定

### 処理フロー（共通）
```
Engine::run(outputPath)
  ├── Logger::log("Engine: Starting processing")
  ├── DataManager::process()
  │     ├── Logger::log("DataManager: Starting data processing")
  │     ├── CalculationManager::calculate()
  │     │     └── Logger::log("CalculationManager: Calculating sum")
  │     └── StatisticsManager::analyze(calcResult)
  │           └── Logger::log("StatisticsManager: Analyzing value")
  └── FileManager::writeToFile(outputPath, result)
        └── Logger::log("FileManager: Writing to file")
```

## Dependency Injection (DI) の仕組み

### DIとは？

Dependency Injection（依存性注入）は、オブジェクトが必要とする依存オブジェクトを外部から注入するデザインパターンです。

### 従来の方法（origin版）の問題点

```cpp
// Engine.cpp - 従来の方法
Engine::Engine()
    : logger_(std::make_unique<Logger>())  // Loggerに依存
{
    // DataManagerを直接生成 - DataManagerの実装に依存
    dataManager_ = std::make_unique<DataManager>(*logger_);

    auto initConfig = initialize();

    // FileManagerを直接生成 - FileManagerの実装に依存
    fileManager_ = std::make_unique<FileManager>(initConfig, *logger_);
}
```

**問題点**:
1. **密結合**: Engineが具象クラス（DataManager、FileManager）に直接依存
2. **テスト困難**: モックやスタブに差し替えるのが難しい
3. **初期化順序の管理**: 手動で依存関係の順序を管理する必要がある
4. **変更の影響**: 依存クラスのコンストラクタが変わるとEngineも修正が必要

### DI版の利点

```cpp
// Engine.cpp - DI版
Engine::Engine(
    std::shared_ptr<Logger> logger,           // 注入される
    std::shared_ptr<DataManager> dataManager, // 注入される
    std::shared_ptr<FileManager> fileManager  // 注入される
)
    : logger_(logger)
    , dataManager_(dataManager)
    , fileManager_(fileManager)
{
    logger_->log("Engine: Initialized");
}
```

**利点**:
1. **疎結合**: Engineは注入されたオブジェクトのインターフェースのみに依存
2. **テスト容易**: モックオブジェクトを簡単に注入可能
3. **自動解決**: DIコンテナが依存関係を自動的に解決
4. **設定の集中化**: `di_module.h`で全ての依存関係を一元管理

### Boost.DIの設定（di/di_module.h）

```cpp
inline auto createModule()
{
    namespace bdi = boost::di;

    return bdi::make_injector(
        // Loggerをシングルトンとして提供
        // → 全クラスで同じLoggerインスタンスを共有
        bdi::bind<Logger>.in(bdi::singleton),

        // SessionIdをファクトリ関数から生成
        // → 実行時に動的に値を生成して注入
        bdi::bind<std::string>.to(createSessionId())
    );
}
```

### DIコンテナの動作

1. **依存関係の解析**（コンパイル時）:
   ```
   Engine が必要とするもの:
     - std::shared_ptr<Logger>
     - std::shared_ptr<DataManager>
     - std::shared_ptr<FileManager>

   DataManager が必要とするもの:
     - std::shared_ptr<Logger>
     - std::shared_ptr<CalculationManager>
     - std::shared_ptr<StatisticsManager>

   ... (以下同様に解析)
   ```

2. **生成順序の決定**（コンパイル時）:
   ```
   依存関係グラフから生成順序を自動決定:
   1. Logger (シングルトン)
   2. CalculationManager(logger)
   3. StatisticsManager(logger)
   4. DataManager(logger, calc, stats)
   5. FileManager(sessionId, logger)
   6. Engine(logger, dataManager, fileManager)
   ```

3. **オブジェクトの生成**（実行時）:
   ```cpp
   auto engine = injector.create<std::shared_ptr<di::Engine>>();
   // ↑ この1行で上記の全てが自動実行される
   ```

### DIが前方宣言だけでは動作しない理由

Boost.DIはコンパイル時に依存関係を解決するため、完全な型定義が必要です:

```cpp
// ❌ これはコンパイルエラー
// engine.h
class DataManager;  // 前方宣言のみ

class Engine {
    Engine(std::shared_ptr<DataManager> dm);  // 型が不完全
};

// ✅ 正しい方法
// engine.h
#include "data_manager.h"  // 完全な定義が必要

class Engine {
    Engine(std::shared_ptr<DataManager> dm);
};
```

**理由**:
- DIコンテナは`__is_polymorphic`などの型特性（type traits）を使用
- コンストラクタの引数型の完全な情報が必要
- 依存関係グラフを完全に構築する必要がある

## 依存関係の特徴

### origin版
- **Logger**: Engine → DataManager → CalculationManager/StatisticsManager
  - Engineがunique_ptrで所有
  - 参照（`Logger&`）を各クラスに渡す
  - 生存期間をEngineが管理

- **初期化順序**:
  1. Logger生成
  2. DataManager生成（Loggerへの参照を渡す）
  3. SessionId生成
  4. FileManager生成（SessionIdとLoggerへの参照を渡す）

### DI版
- **Logger**: シングルトンとして全クラスで共有
  - DIコンテナが生存期間を管理
  - 全クラスがshared_ptrで同じインスタンスを参照

- **初期化順序**: DIコンテナが自動決定
  - コンストラクタの引数から依存関係を自動解析
  - トポロジカルソートで生成順序を決定
  - 循環依存があればコンパイルエラー

## 2つのバージョンの詳細比較

### origin版（従来の方法）

```cpp
class Engine {
    Engine() {
        logger_ = std::make_unique<Logger>();
        dataManager_ = std::make_unique<DataManager>(*logger_);
        auto sessionId = initialize();
        fileManager_ = std::make_unique<FileManager>(sessionId, *logger_);
    }
};

class FileManager {
    FileManager(const std::string& sessionId, Logger& logger);
    // コンストラクタで全ての値を受け取る
};
```

**特徴**:
- 自由な設計が可能
- コンストラクタで実行時の値も受け取れる
- 生成順序を明示的に制御
- 依存オブジェクトを直接生成（密結合）

### di版（DI向けの設計）

```cpp
class Engine {
    Engine(shared_ptr<Logger> logger,
           shared_ptr<DataManager> dataManager,
           shared_ptr<FileManager> fileManager) {
        auto sessionId = generateSessionId();
        fileManager->setSessionId(sessionId);  // 後から設定
    }
};

class FileManager {
    FileManager(std::shared_ptr<Logger> logger);  // 依存のみ
    void setSessionId(const std::string& sessionId);  // setter追加
};
```

**特徴**:
- コンストラクタは依存オブジェクトのみ（DI原則）
- 実行時の値はsetterで後から設定
- DIコンテナが依存を自動解決
- シンプルなDIモジュール

## DIモジュール

```cpp
return bdi::make_injector(
    bdi::bind<Logger>.in(bdi::singleton)
    // これだけ！他のクラスは全て自動解決される
);
```

**ポイント**:
- Loggerだけシングルトンを指定
- 他のクラスは自動的にデフォルトスコープ（毎回新規生成）で解決される
- 非常にシンプル

## DIの良し悪し

### DIのメリット

#### 1. **疎結合**
```cpp
// ❌ 密結合（origin）
class Engine {
    Engine() {
        dataManager_ = std::make_unique<DataManager>(*logger_);
        // ↑ DataManagerの具象クラスに直接依存
    }
};

// ✅ 疎結合（DI）
class Engine {
    Engine(std::shared_ptr<DataManager> dataManager)
        : dataManager_(dataManager) {}
    // ↑ 外部から注入される。DataManagerの生成方法を知らない
};
```

#### 2. **テスト容易性**
```cpp
// テスト時にモックを注入できる
auto mockDataManager = std::make_shared<MockDataManager>();
auto mockFileManager = std::make_shared<MockFileManager>();
Engine engine(logger, mockDataManager, mockFileManager);
// ↑ 本物の代わりにモックを注入
```

#### 3. **依存関係の可視化**
```cpp
// コンストラクタを見れば依存関係が一目瞭然
Engine(
    std::shared_ptr<Logger> logger,           // Loggerに依存
    std::shared_ptr<DataManager> dataManager, // DataManagerに依存
    std::shared_ptr<FileManager> fileManager  // FileManagerに依存
);
```

#### 4. **ライフサイクル管理の集中化**
```cpp
return bdi::make_injector(
    bdi::bind<Logger>.in(bdi::singleton),      // シングルトン
    bdi::bind<DataManager>.in(bdi::unique)     // 毎回新規生成
);
// ↑ 一箇所で全てのライフサイクルを管理
```

### DIのデメリット

#### 1. **学習コストが高い**
- DIコンテナの概念を理解する必要がある
- Boost.DIの文法を学ぶ必要がある
- 依存関係グラフの考え方が必要

#### 2. **設計の制約**
```cpp
// ❌ DIに不向き
class FileManager {
    FileManager(const std::string& sessionId, Logger& logger);
    // 実行時に計算される値をコンストラクタで受け取る
};

// ✅ DIに向く
class FileManager {
    FileManager(std::shared_ptr<Logger> logger);  // 依存のみ
    void setSessionId(const std::string& id);      // 値は後から設定
};
```

**コンストラクタで実行時の値を受け取る設計は避けるべき**

#### 3. **複雑なバインディングが必要になる場合がある**
```cpp
// 特定のクラスにだけ特別な値を注入したい場合
return bdi::make_injector(
    bdi::bind<Logger>.in(bdi::singleton),

    // ❌ 複雑になりがち
    bdi::bind<FileManager>.to([](const auto& injector) -> FileManager {
        return FileManager(
            createSessionId(),  // 特別な処理
            injector.template create<std::shared_ptr<Logger>>()
        );
    })
);
```

**この複雑さは設計の問題を示すサイン**

#### 4. **前方宣言が使えない**
```cpp
// ❌ コンパイルエラー
class DataManager;  // 前方宣言のみ

class Engine {
    Engine(std::shared_ptr<DataManager> dm);  // 型が不完全
};

// ✅ 完全な定義が必要
#include "data_manager.h"  // 完全な定義をインクルード
```

DIはコンパイル時に型の完全な情報が必要

#### 5. **デバッグが難しい場合がある**
- オブジェクトの生成がDIコンテナ内で行われる
- スタックトレースが複雑になる
- コンパイルエラーメッセージが長大

### DIを使うべき場合

✅ **使うべき**:
1. 大規模なプロジェクト（依存関係が複雑）
2. テストを重視するプロジェクト
3. プラグインシステムなど、実行時に実装を切り替える必要がある
4. チーム開発で依存関係を明確にしたい

❌ **使わなくてもいい**:
1. 小規模なプロジェクト（数十クラス以下）
2. 依存関係がシンプル
3. 実行時の切り替えが不要
4. プロトタイピング段階

### DIを使う場合の設計原則

#### 原則1: コンストラクタは依存オブジェクトのみ

```cpp
// ✅ 良い
class MyClass {
    MyClass(std::shared_ptr<Logger> logger, std::shared_ptr<Config> config);
};

// ❌ 悪い
class MyClass {
    MyClass(const std::string& sessionId, std::shared_ptr<Logger> logger);
    // sessionIdは実行時に生成される値
};
```

#### 原則2: 実行時の値はsetter/初期化メソッドで設定

```cpp
class MyClass {
    MyClass(std::shared_ptr<Logger> logger);  // コンストラクタ: 依存のみ
    void setSessionId(const std::string& id); // 初期化メソッド: 実行時の値
};
```

#### 原則3: 設定値はConfigクラスにまとめる

```cpp
// ❌ 個別にバインド（複雑）
bdi::bind<int>.named<PortTag>().to(8080);
bdi::bind<std::string>.named<HostTag>().to("localhost");

// ✅ Configクラスにまとめる（シンプル）
class Config {
    Config();  // config.iniから読み込む
    int getPort() const;
    std::string getHost() const;
};

bdi::bind<Config>.in(bdi::singleton);
```

#### 原則4: DIモジュールはシンプルに保つ

```cpp
// ✅ 理想的
return bdi::make_injector(
    bdi::bind<Logger>.in(bdi::singleton),
    bdi::bind<Config>.in(bdi::singleton)
);

// ❌ 複雑なバインディングが必要なら設計を見直す
return bdi::make_injector(
    bdi::bind<Logger>.in(bdi::singleton),
    bdi::bind<FileManager>.to([](const auto& injector) -> FileManager {
        // 複雑な処理...
    })
);
```

**複雑なバインディングが必要 = 設計がDIに向いていない**

## まとめ

### 各バージョンの使い分け

| バージョン | 適用場面 |
|----------|---------|
| **origin** | 小規模プロジェクト、プロトタイピング、学習目的 |
| **di** | 本格的にDIを活用する場合（テスト重視、大規模プロジェクト） |

### 重要な教訓

1. **DIは銀の弾丸ではない**
   - 全てのプロジェクトに必要なわけではない
   - 小規模ならoriginの方がシンプルで良い

2. **DIを使うなら設計から見直す**
   - コンストラクタ = 依存オブジェクトのみ
   - 実行時の値 = setter/初期化メソッド
   - この原則を守らないとDIモジュールが複雑になる

3. **複雑さは設計の問題**
   - DIモジュールが複雑になったら設計を見直す
   - 無理にDIを使わない方が良い場合もある

4. **2つのバージョンを比較することで理解が深まる**
   - origin: 基準となるシンプルな実装
   - di: DIに適した設計に変更した実装

本プロジェクトでは2つの実装を並べることで、DIの利点と欠点、そして適切な設計方法を理解できるようになっています。
