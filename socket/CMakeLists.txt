# socket ディレクトリ用のCMakeLists.txt

# エコーサーバー・クライアントセット
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

# C++23を設定
set_target_properties(echo_server echo_client
    PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)