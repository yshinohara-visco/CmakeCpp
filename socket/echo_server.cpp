#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection> {
public:
    Connection(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        std::cout << "新しいクライアント接続: " << socket_.remote_endpoint() << std::endl;
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string message(data_, length);
                    std::cout << "受信: " << message;
                    
                    // 応答をwrite_buffer_に格納
                    write_buffer_ = "エコー: " + message;
                    do_write();
                } else {
                    std::cout << "クライアント切断: " << socket_.remote_endpoint() << std::endl;
                }
            });
    }

    void do_write() {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(write_buffer_),
            [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    std::cout << "送信完了: " << length << " バイト" << std::endl;
                    do_read(); // 次のメッセージを待機
                } else {
                    std::cout << "送信エラー: " << ec.message() << std::endl;
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    std::string write_buffer_;  // 送信用バッファを追加
};

class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        std::cout << "エコーサーバー開始 - ポート: " << port << std::endl;
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](std::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Connection>(std::move(socket))->start();
                } else {
                    std::cout << "接続受付エラー: " << ec.message() << std::endl;
                }
                do_accept(); // 次の接続を待機
            });
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        Server s(io_context, 8080);
        std::cout << "サーバー起動完了。Ctrl+Cで終了してください。" << std::endl;
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "エラー: " << e.what() << std::endl;
    }

    return 0;
}