
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
find_package(Boost REQUIRED)

# ソースファイル
target_sources(${TARGET_NAME}
	PRIVATE
		main.cpp
		origin/engine.h
		origin/engine.cpp
		origin/data_manager.h
		origin/data_manager.cpp
		origin/calculation_manager.h
		origin/calculation_manager.cpp
		origin/statistics_manager.h
		origin/statistics_manager.cpp
		origin/file_manager.h
		origin/file_manager.cpp
		origin/logger.h
		origin/logger.cpp
		di/engine.h
		di/engine.cpp
		di/data_manager.h
		di/data_manager.cpp
		di/calculation_manager.h
		di/calculation_manager.cpp
		di/statistics_manager.h
		di/statistics_manager.cpp
		di/file_manager.h
		di/file_manager.cpp
		di/logger.h
		di/logger.cpp
		di/di_module.h
		boost/di.h)

# ライブラリ
target_link_libraries(${TARGET_NAME}
	PRIVATE
		Boost::headers
)