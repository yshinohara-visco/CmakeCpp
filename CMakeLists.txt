﻿
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.14)

# プロジェクト名
project(CmakeCpp)

# 出力するバイナリ
set (TARGET_NAME CmakeCpp)
add_executable(${TARGET_NAME})

# C++23を使用する
set_target_properties(${TARGET_NAME}
	PROPERTIES
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
)

# ソースファイル
target_sources(${TARGET_NAME}
	PRIVATE
		main.cpp
)