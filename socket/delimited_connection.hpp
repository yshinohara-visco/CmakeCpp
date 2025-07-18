#pragma once
#include "connection_base.hpp"
#include <boost/asio.hpp>
#include <string>

// 区切り文字を使用した通信プロトコル
class DelimitedConnection : public ConnectionBase {
public:
    DelimitedConnection(tcp::socket socket, char delimiter = '\n');

protected:
    void start_receive() override;
    void handle_receive(const boost::system::error_code& ec, std::size_t bytes_received) override;
    void prepare_send_data(const std::string& message) override;

private:
    void process_received_data();
    
    char delimiter_;                    // 区切り文字
    std::string incomplete_message_;    // 未完成メッセージバッファ
};

// 区切り文字プロトコル用サーバー
class DelimitedServer : public ServerBase<DelimitedConnection> {
public:
    DelimitedServer(boost::asio::io_context& io_context, short port, char delimiter = '\n');

protected:
    void on_new_connection(std::shared_ptr<DelimitedConnection> connection) override;
    void on_connection_error(const std::string& error) override;

private:
    void setup_connection_handlers(std::shared_ptr<DelimitedConnection> connection);
    
    char delimiter_;
};