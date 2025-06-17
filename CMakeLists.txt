
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.00)

# プロジェクト名
project(CmakeCpp)

# 出力するバイナリ
set (TARGET_NAME CmakeCpp)
add_executable(${TARGET_NAME})

# C++17を使用する
set_target_properties(${TARGET_NAME}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

# ソースファイル
target_sources(${TARGET_NAME}
	PRIVATE
		main.cpp
 "DriveChecker1.cpp" "DriveChecker1.h" "common.h" "common.cpp" "DriveChecker2.h" "DriveChecker2.cpp" "DriveChecker3.h" "DriveChecker3.cpp" "DriveChecker4.h" "DriveChecker4.cpp")

 target_link_libraries(${TARGET_NAME}
 PRIVATE
		Mpr
		Shlwapi
	)