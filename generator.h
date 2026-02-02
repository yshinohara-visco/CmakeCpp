#pragma once

// VisualStudio2022 17.12.8 ではincludeできなかった
// VisualStudio2026 では使用可能
#include <generator>
#include <cstdint>
#include <string>

// フィボナッチ数列を無限に生成するジェネレータ
std::generator<uint64_t> fibonacci();

// チェックポイント付き処理のジェネレータ
// 各処理の完了時にチェックポイント名をyieldする
std::generator<std::string> checkpointProcess();
