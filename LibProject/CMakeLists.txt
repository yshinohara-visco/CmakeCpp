
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.00)

# プロジェクト名
project(LibProject)

# サブディレクトリ
set(LIB_SHARED "ModuleShared")
set(LIB_STATIC "ModuleStatic")
add_subdirectory(ModuleStatic)
add_subdirectory(ModuleShared)


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
)

# リンクするライブラリ
target_link_libraries(${RUN_MAIN}
	PRIVATE
		ModuleStatic
		ModuleShared
)

# インストール
install(
	TARGETS 
		${RUN_MAIN}
		${LIB_STATIC}
		${LIB_SHARED}
	RUNTIME
		COMPONENT Runtime
	LIBRARY
		COMPONENT Runtime
		NAMELINK_COMPONENT Development
	ARCHIVE
		COMPONENT Development
	FILE_SET HEADERS
		 COMPONENT Development
)