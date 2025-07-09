# 非同期データアクセスライブラリ
add_library(async_data
    common.h
    async_data.h
    async_data.cpp
    async_data_async.h
    async_data_async.cpp
)

target_include_directories(async_data PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})