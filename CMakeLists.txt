
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.00)

# プロジェクト名
project(CmakeCpp)

# dllに全てのシンボルをエクスポートする
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# ターゲットの追加
set (TARGET_LIB hello)
add_library(${TARGET_LIB} SHARED)

# C++17を使用する
set_target_properties(${TARGET_LIB}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

# includeパスの公開
target_include_directories(${TARGET_LIB} PUBLIC "include")

# サブディレクトリ
add_subdirectory(src)
add_subdirectory(test)

# インストール
install(TARGETS ${TARGET_LIB}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
# ヘッダーファイルのインストール ターゲットに入れて上記コマンドで一緒にできないか？
install(DIRECTORY "include" DESTINATION ${CMAKE_INSTALL_PREFIX})