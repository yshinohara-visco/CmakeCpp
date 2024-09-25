
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.00)

# バージョン
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# プロジェクト名
project(CmakeProject LANGUAGES CXX VERSION ${VERSION})

# rcファイル生成
configure_file(
	${CMAKE_SOURCE_DIR}/version.rc.in
	${CMAKE_BINARY_DIR}/version.rc
	@ONLY
)

# 出力するバイナリ
set (TARGET_NAME CmakeExe)
add_executable(${TARGET_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

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
		${CMAKE_BINARY_DIR}/version.rc
)

# リンク
target_link_libraries(${TARGET_NAME}
	PRIVATE
		CmakeDll
)

# インストール
install(TARGETS ${TARGET_NAME}
	RUNTIME DESTINATION bin
)

add_subdirectory(lib)