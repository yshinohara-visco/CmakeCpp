# GoogleTest設定ファイル
# このファイルはGoogleTestの取得・設定・テスト機能を提供します

# FetchContentモジュールを有効にする
include(FetchContent)

# GoogleTestを取得・設定
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# テスト有効化
enable_testing()

# GoogleTestヘルパー関数を提供
include(GoogleTest)

# GoogleTest用のターゲット作成ヘルパー関数
function(create_gtest_target TARGET_NAME SOURCE_FILES LINK_LIBRARIES)
    # テスト実行ファイル
    add_executable(${TARGET_NAME} ${SOURCE_FILES})
    
    # C++23を使用する
    set_target_properties(${TARGET_NAME}
        PROPERTIES
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON
    )
    
    # ライブラリとGoogleTestをリンク
    target_link_libraries(${TARGET_NAME}
        ${LINK_LIBRARIES}
        gtest_main
    )
    
    # Google Testのテストを登録
    gtest_discover_tests(${TARGET_NAME})
endfunction()