
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.00)

# プロジェクト名
project(LibProject)

# 出力するバイナリ
set (RUN_MAIN CmakeCpp)
add_executable(${RUN_MAIN})

# C++17を使用する
set_target_properties(${RUN_MAIN}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

# ソースファイル
target_sources(${RUN_MAIN}
	PRIVATE
		main.cpp 
  "ModuleShared.h")

# リンクするライブラリ
target_link_libraries(${RUN_MAIN}
	PRIVATE
		ModuleShared
)

# インストール
install(
	TARGETS 
		${RUN_MAIN}
)