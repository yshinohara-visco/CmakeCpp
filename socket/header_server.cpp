#include "header_connection.hpp"
#include <iostream>

int main() {
    try {
        boost::asio::io_context io_context;
        
        // 固定長ヘッダーサーバーを作成（ポート8082）
        HeaderServer server(io_context, 8082);
        
        std::cout << "固定長ヘッダーサーバー起動完了。Ctrl+Cで終了してください。" << std::endl;
        std::cout << "プロトコル形式: [4バイトID][4バイトサイズ][メッセージ本体]" << std::endl;
        std::cout << "テスト用にheader_clientを実行してください。" << std::endl;
        
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "エラー: " << e.what() << std::endl;
    }

    return 0;
}