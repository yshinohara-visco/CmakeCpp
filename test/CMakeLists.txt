# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.00)

# プロジェクト名
project(TestHello)

# ターゲットの追加
set (TARGET_EXE test_hello)
add_executable(${TARGET_EXE})

# C++17を使用する
set_target_properties(${TARGET_NAME}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

# ソースの追加
target_sources(${TARGET_EXE} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# 使用するライブラリの設定
target_link_libraries(${TARGET_EXE} PRIVATE ${TARGET_LIB})

# includeパスはlibでpublicで設定したのを取り込んだ

install(TARGETS ${TARGET_EXE}
	RUNTIME DESTINATION bin
)