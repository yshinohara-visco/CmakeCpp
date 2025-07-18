#include "delimited_connection.hpp"
#include <iostream>

int main() {
    try {
        boost::asio::io_context io_context;
        
        // 改行区切りサーバーを作成（ポート8081）
        DelimitedServer server(io_context, 8081, '\n');
        
        std::cout << "区切り文字サーバー起動完了。Ctrl+Cで終了してください。" << std::endl;
        std::cout << "テスト方法: telnet localhost 8081" << std::endl;
        std::cout << "メッセージを入力してEnterを押してください。" << std::endl;
        
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "エラー: " << e.what() << std::endl;
    }

    return 0;
}