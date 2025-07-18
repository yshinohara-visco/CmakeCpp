#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <functional>

using boost::asio::ip::tcp;

// メッセージハンドラーの型定義
using MessageHandler = std::function<void(const std::string&)>;
using ErrorHandler = std::function<void(const std::string&)>;

// 抽象基底クラス
class ConnectionBase : public std::enable_shared_from_this<ConnectionBase> {
public:
    ConnectionBase(tcp::socket socket);
    virtual ~ConnectionBase() = default;

    // 外部インターフェース
    void start();
    void send_message(const std::string& message);
    void disconnect();
    
    // コールバック設定
    void set_message_handler(MessageHandler handler) { message_handler_ = std::move(handler); }
    void set_error_handler(ErrorHandler handler) { error_handler_ = std::move(handler); }
    
    // 接続情報
    tcp::endpoint get_remote_endpoint() const;
    bool is_connected() const { return connected_; }

protected:
    // 派生クラスで実装する純粋仮想関数
    virtual void start_receive() = 0;
    virtual void handle_receive(const boost::system::error_code& ec, std::size_t bytes_received) = 0;
    virtual void prepare_send_data(const std::string& message) = 0;

    // 共通の送受信処理
    void start_send();
    void handle_send(const boost::system::error_code& ec, std::size_t bytes_sent);
    void handle_error(const std::string& error_msg);
    void close_connection();

    // 共通メンバー変数
    tcp::socket socket_;
    bool connected_;
    std::array<char, 8192> receive_buffer_;  // 受信バッファ
    std::string send_buffer_;               // 送信バッファ
    
    // コールバック
    MessageHandler message_handler_;
    ErrorHandler error_handler_;

private:
    void on_start();
};

// サーバー基底クラス
template<typename ConnectionType>
class ServerBase {
public:
    ServerBase(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

    virtual ~ServerBase() = default;

protected:
    virtual void on_new_connection(std::shared_ptr<ConnectionType> connection) {}
    virtual void on_connection_error(const std::string& error) {}

private:
    void start_accept() {
        acceptor_.async_accept(
            [this](std::error_code ec, tcp::socket socket) {
                if (!ec) {
                    auto connection = std::make_shared<ConnectionType>(std::move(socket));
                    on_new_connection(connection);
                    connection->start();
                } else {
                    on_connection_error("Accept error: " + ec.message());
                }
                start_accept();
            });
    }

    tcp::acceptor acceptor_;
};