
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

# boost
set(CMAKE_PREFIX_PATH "D:/Library/boost") # インストール場所
find_package(Boost REQUIRED COMPONENTS system)

# ソースファイル
target_sources(${TARGET_NAME}
	PRIVATE
		main.cpp
)

# サブディレクトリの追加
add_subdirectory(io_context)
add_subdirectory(socket)

# ライブラリ
target_link_libraries(${TARGET_NAME}
	PRIVATE
		Boost::headers
)