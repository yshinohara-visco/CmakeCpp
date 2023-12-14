
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.00)

# プロジェクト名
project(HelloWorld)

# 出力するバイナリ
add_executable(HelloWorld)

# C++17を使用する
set_target_properties(HelloWorld
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

# ソースファイル
target_sources(HelloWorld
	PRIVATE
		main.cpp
)