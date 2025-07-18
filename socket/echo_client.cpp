#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        
        // エコーサーバーに接続
        std::cout << "エコーサーバーに接続中..." << std::endl;
        boost::asio::connect(socket, resolver.resolve("localhost", "8080"));
        std::cout << "エコーサーバーに接続しました" << std::endl;
        
        // メッセージ送信
        std::string message = "こんにちは、サーバー！\n";
        boost::asio::write(socket, boost::asio::buffer(message));
        std::cout << "送信: " << message;
        
        // 応答受信（read_someを使用）
        std::array<char, 1024> reply;
        size_t reply_length = socket.read_some(boost::asio::buffer(reply));
        std::cout << "受信: ";
        std::cout.write(reply.data(), reply_length);
        
        // 2回目のメッセージ
        message = "2回目のメッセージです\n";
        boost::asio::write(socket, boost::asio::buffer(message));
        std::cout << "送信: " << message;
        
        reply_length = socket.read_some(boost::asio::buffer(reply));
        std::cout << "受信: ";
        std::cout.write(reply.data(), reply_length);
        
        std::cout << "通信完了" << std::endl;
        
    } catch (std::exception& e) {
        std::cerr << "エラー: " << e.what() << std::endl;
    }
    
    return 0;
}