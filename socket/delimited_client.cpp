#include "delimited_connection.hpp"
#include <iostream>
#include <thread>
#include <chrono>

class DelimitedClient {
public:
    DelimitedClient(boost::asio::io_context& io_context)
        : io_context_(io_context), socket_(io_context) {}

    void connect(const std::string& host, const std::string& port) {
        tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, port);
        
        boost::asio::connect(socket_, endpoints);
        std::cout << "区切り文字サーバーに接続しました" << std::endl;
        
        // 接続オブジェクトを作成
        connection_ = std::make_shared<DelimitedConnection>(std::move(socket_), '\n');
        
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
    std::shared_ptr<DelimitedConnection> connection_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        DelimitedClient client(io_context);
        
        // サーバーに接続
        std::cout << "区切り文字サーバーに接続中..." << std::endl;
        client.connect("localhost", "8081");
        
        // io_contextを別スレッドで実行
        std::thread io_thread([&io_context]() {
            io_context.run();
        });
        
        // メッセージ送信テスト
        std::cout << "メッセージ送信テスト開始" << std::endl;
        
        client.send_message("こんにちは、区切り文字プロトコル！");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        client.send_message("区切り文字で複数のメッセージを送信");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        client.send_message("改行文字で自動的に分割されます");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        client.send_message("最後のメッセージです");
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