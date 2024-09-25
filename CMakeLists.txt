
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.00)

# プロジェクト名
project(CmakeCpp LANGUAGES CXX VERSION 1.0.0)

# 出力するバイナリ
set (TARGET_NAME CmakeCpp)
add_executable(${TARGET_NAME})

# バージョン
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${VERSION})

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
)

# リンク
target_link_libraries(${TARGET_NAME}
	PRIVATE
		Hello
)

# インストール
install(TARGETS ${TARGET_NAME}
	RUNTIME DESTINATION bin
)

add_subdirectory(lib)