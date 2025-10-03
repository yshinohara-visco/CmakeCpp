# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## プロジェクト目的

このプロジェクトは、ClaudeCode for VSCodeを使って**編集・ビルド・実行・動作確認のサイクルを単体で完結**できることを検証するための実験的プロジェクトです。端末に出力を行うプログラムを作成し、その出力を元にClaudeCodeが動作確認と継続的な開発を行います。

## 開発コマンド

### CMake設定
```bash
cmake -B build -S .
```

### ビルド (Release構成)
```bash
cmake --build build --config Release
```

### 実行
```bash
./build/Release/CmakeCpp.exe
```

## プロジェクト構成

- **言語**: C++23
- **ビルドシステム**: CMake 3.14+
- **環境**: Windows (Visual Studio 2022 / MSBuild)
- **実行ファイル**: `CmakeCpp.exe` (build/Release/ 配下に生成)

## 開発方針

- 処理結果や実行中の情報を**必ず端末に出力**すること
- 出力を元にClaudeCodeが動作確認できるように設計すること
