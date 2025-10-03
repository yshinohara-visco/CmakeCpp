
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.14)

# プロジェクト名
project(CmakeCpp)

# C++23を使用する
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# メイン実行ファイル
set (TARGET_NAME CmakeCpp)
add_executable(${TARGET_NAME})

set_target_properties(${TARGET_NAME}
	PROPERTIES
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
)

target_sources(${TARGET_NAME}
	PRIVATE
		main.cpp
)

# テスト実行ファイル
add_executable(calculator_test test/calculator_test.cpp)
target_compile_options(calculator_test PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)