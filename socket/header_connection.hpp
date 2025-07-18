#pragma once
#include "connection_base.hpp"
#include <boost/asio.hpp>
#include <cstdint>

// 固定長ヘッダー（ID + サイズ）を使用した通信プロトコル
class HeaderConnection : public ConnectionBase {
public:
    struct MessageHeader {
        uint32_t message_id;    // メッセージID（ネットワークバイトオーダー）
        uint32_t message_size;  // メッセージサイズ（ネットワークバイトオーダー）
        
        static constexpr size_t HEADER_SIZE = sizeof(message_id) + sizeof(message_size);
    };

    HeaderConnection(tcp::socket socket);

protected:
    void start_receive() override;
    void handle_receive(const boost::system::error_code& ec, std::size_t bytes_received) override;
    void prepare_send_data(const std::string& message) override;

private:
    void start_receive_header();
    void handle_receive_header(const boost::system::error_code& ec, std::size_t bytes_received);
    void start_receive_body();
    void handle_receive_body(const boost::system::error_code& ec, std::size_t bytes_received);
    
    uint32_t host_to_network(uint32_t value);
    uint32_t network_to_host(uint32_t value);

    enum class ReceiveState {
        WAITING_HEADER,
        WAITING_BODY
    };

    ReceiveState receive_state_;
    MessageHeader current_header_;
    std::vector<char> message_body_;
    uint32_t next_message_id_;       // 送信用メッセージID
};

// 固定長ヘッダープロトコル用サーバー
class HeaderServer : public ServerBase<HeaderConnection> {
public:
    HeaderServer(boost::asio::io_context& io_context, short port);

protected:
    void on_new_connection(std::shared_ptr<HeaderConnection> connection) override;
    void on_connection_error(const std::string& error) override;

private:
    void setup_connection_handlers(std::shared_ptr<HeaderConnection> connection);
};