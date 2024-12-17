
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.00)

# プロジェクト名
project(CmakeCpp)

# 出力するバイナリ
set (TARGET_NAME CmakeCpp)
add_executable(${TARGET_NAME})

# C++20を使用する
set_target_properties(${TARGET_NAME}
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
)

# ソースファイル
target_sources(${TARGET_NAME}
	PRIVATE
		main.cpp
 "precise_wait.cpp" "precise_wait.h" "stop_watch.h" "stop_watch.cpp")

 target_link_libraries(${TARGET_NAME} PRIVATE winmm)