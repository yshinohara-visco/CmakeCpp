# io_contextサンプル用のCMakeLists.txt

# io_contextサンプル用の実行ファイル
add_executable(basic_example basic_example.cpp)
add_executable(timer_example timer_example.cpp)
add_executable(async_example async_example.cpp)

# C++23を設定
set_target_properties(basic_example timer_example async_example
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
)

# io_contextサンプルにBoostをリンク
target_link_libraries(basic_example PRIVATE Boost::headers Boost::system)
target_link_libraries(timer_example PRIVATE Boost::headers Boost::system)
target_link_libraries(async_example PRIVATE Boost::headers Boost::system)

# 出力ディレクトリを設定（親プロジェクトと同じ場所に出力）
set_target_properties(basic_example timer_example async_example
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)