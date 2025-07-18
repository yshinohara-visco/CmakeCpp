# socket ディレクトリ用のCMakeLists.txt

# 共通基底クラスライブラリ
add_library(connection_base 
    connection_base.cpp
    delimited_connection.cpp 
    header_connection.cpp
)
target_link_libraries(connection_base 
    PUBLIC 
    Boost::system
)

# エコーサーバー・クライアントセット（動作確認済み）
add_executable(echo_server echo_server.cpp)
target_link_libraries(echo_server 
    PRIVATE 
    Boost::system
)

add_executable(echo_client echo_client.cpp)
target_link_libraries(echo_client 
    PRIVATE 
    Boost::system
)

# 区切り文字プロトコル
add_executable(delimited_server delimited_server.cpp)
target_link_libraries(delimited_server 
    PRIVATE 
    connection_base
)

add_executable(delimited_client delimited_client.cpp)
target_link_libraries(delimited_client 
    PRIVATE 
    connection_base
)

# 固定長ヘッダープロトコル
add_executable(header_server header_server.cpp)
target_link_libraries(header_server 
    PRIVATE 
    connection_base
)

add_executable(header_client header_client.cpp)
target_link_libraries(header_client 
    PRIVATE 
    connection_base
)

# C++23を設定
set_target_properties(
    connection_base
    echo_server echo_client
    delimited_server delimited_client
    header_server header_client
    PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)