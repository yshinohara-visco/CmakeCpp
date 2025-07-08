
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.14)

# プロジェクト名
project(CmakeCpp)

# GoogleTest設定を読み込み
include(cmakes/google_test.cmake)

# 出力するバイナリ
set (TARGET_NAME CmakeCpp)
add_executable(${TARGET_NAME})

# C++23を使用する
set_target_properties(${TARGET_NAME}
	PROPERTIES
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
)

# サブディレクトリ
add_subdirectory(math_lib)
add_subdirectory(tests)

# ソースファイル
target_sources(${TARGET_NAME}
	PRIVATE
		main.cpp
)

# リンクライブラリ
target_link_libraries(${TARGET_NAME}
	PRIVATE
		math_operations
)