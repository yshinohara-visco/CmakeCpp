
# cmakeに要求する最低バージョン
cmake_minimum_required(VERSION 3.00)

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

# VisualStudio2022内のstd.ixxをビルド対象に含めないと標準ライブラリモジュールが使えない
# フルパスを直に書くとインストール場所が異なる人はビルドできない
# 動的に取得できないか？

# ソースファイル
target_sources(${TARGET_NAME}
	PRIVATE
		"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.37.32822/modules/std.ixx"
		main.cpp
		"hello.ixx"
)