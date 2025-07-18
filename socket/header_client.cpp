#include "header_connection.hpp"
#include <iostream>
#include <thread>
#include <chrono>

class HeaderClient {
public:
    HeaderClient(boost::asio::io_context& io_context)
        : io_context_(io_context), socket_(io_context) {}

    void connect(const std::string& host, const std::string& port) {
        tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, port);
        
        boost::asio::connect(socket_, endpoints);
        std::cout << "固定長ヘッダーサーバーに接続しました" << std::endl;
        
        // 接続オブジェクトを作成
        connection_ = std::make_shared<HeaderConnection>(std::move(socket_));
        
        // レスポンス受信用ハンドラー設定
        connection_->set_message_handler([](const std::string& message) {
            std::cout << "サーバーからの応答: " << message << std::endl;
        });
        
        connection_->set_error_handler([](const std::string& error) {
            std::cout << "接続エラー: " << error << std::endl;
        });
        
        connection_->start();
    }

    void send_message(const std::string& message) {
        if (connection_) {
            connection_->send_message(message);
        }
    }

    void disconnect() {
        if (connection_) {
            connection_->disconnect();
        }
    }

private:
    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    std::shared_ptr<HeaderConnection> connection_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        HeaderClient client(io_context);
        
        // サーバーに接続
        std::cout << "固定長ヘッダーサーバーに接続中..." << std::endl;
        client.connect("localhost", "8082");
        
        // io_contextを別スレッドで実行
        std::thread io_thread([&io_context]() {
            io_context.run();
        });
        
        // メッセージ送信テスト
        std::cout << "メッセージ送信テスト開始" << std::endl;
        
        client.send_message("こんにちは、固定長ヘッダープロトコル！");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        client.send_message("ID付きメッセージ送信");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        // 異なる長さのメッセージをテスト
        client.send_message("短いメッセージ");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        client.send_message("これは比較的長いメッセージです。固定長ヘッダーによりサイズが事前に分かるため、確実に完全なメッセージを受信できます。");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        client.send_message("バイナリデータも送信可能です: \x01\x02\x03\x04");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        std::cout << "テスト完了" << std::endl;
        client.disconnect();
        
        io_context.stop();
        io_thread.join();
        
    } catch (std::exception& e) {
        std::cerr << "エラー: " << e.what() << std::endl;
    }
    
    return 0;
}