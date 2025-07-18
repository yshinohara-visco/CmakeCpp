#include "header_connection.hpp"
#include <iostream>
#include <cstring>

HeaderConnection::HeaderConnection(tcp::socket socket)
    : ConnectionBase(std::move(socket)), 
      receive_state_(ReceiveState::WAITING_HEADER),
      next_message_id_(1) {
}

void HeaderConnection::start_receive() {
    receive_state_ = ReceiveState::WAITING_HEADER;
    start_receive_header();
}

void HeaderConnection::handle_receive(const boost::system::error_code& ec, std::size_t bytes_received) {
    // この関数は使用されない（ヘッダーと本体で個別に処理）
}

void HeaderConnection::start_receive_header() {
    if (!connected_) return;

    auto self = std::static_pointer_cast<HeaderConnection>(shared_from_this());
    boost::asio::async_read(socket_, 
        boost::asio::buffer(&current_header_, MessageHeader::HEADER_SIZE),
        [self](std::error_code ec, std::size_t bytes_received) {
            self->handle_receive_header(ec, bytes_received);
        });
}

void HeaderConnection::handle_receive_header(const boost::system::error_code& ec, std::size_t bytes_received) {
    if (!ec && bytes_received == MessageHeader::HEADER_SIZE) {
        // ネットワークバイトオーダーからホストバイトオーダーに変換
        uint32_t message_id = network_to_host(current_header_.message_id);
        uint32_t message_size = network_to_host(current_header_.message_size);
        
        std::cout << "ヘッダー受信 - ID: " << message_id 
                  << ", サイズ: " << message_size << " バイト" << std::endl;
        
        // サイズ妥当性チェック
        if (message_size > 0 && message_size <= 1024 * 1024) { // 最大1MB
            current_header_.message_id = message_id;
            current_header_.message_size = message_size;
            message_body_.resize(message_size);
            start_receive_body();
        } else {
            handle_error("無効なメッセージサイズ: " + std::to_string(message_size));
        }
    } else {
        if (ec == boost::asio::error::eof) {
            std::cout << "クライアント切断: " << get_remote_endpoint() << std::endl;
        } else {
            handle_error("ヘッダー受信エラー: " + ec.message());
        }
    }
}

void HeaderConnection::start_receive_body() {
    if (!connected_) return;

    auto self = std::static_pointer_cast<HeaderConnection>(shared_from_this());
    boost::asio::async_read(socket_,
        boost::asio::buffer(message_body_),
        [self](std::error_code ec, std::size_t bytes_received) {
            self->handle_receive_body(ec, bytes_received);
        });
}

void HeaderConnection::handle_receive_body(const boost::system::error_code& ec, std::size_t bytes_received) {
    if (!ec && bytes_received == current_header_.message_size) {
        std::string message(message_body_.begin(), message_body_.end());
        std::cout << "メッセージ受信 - ID: " << current_header_.message_id 
                  << ", 内容: " << message << std::endl;
        
        // メッセージハンドラーを呼び出し
        if (message_handler_) {
            message_handler_(message);
        }
        
        // 次のヘッダー受信を開始
        start_receive_header();
    } else {
        handle_error("メッセージ本体受信エラー: " + ec.message());
    }
}

void HeaderConnection::prepare_send_data(const std::string& message) {
    // ヘッダーを作成
    MessageHeader header;
    header.message_id = host_to_network(next_message_id_++);
    header.message_size = host_to_network(static_cast<uint32_t>(message.size()));
    
    // ヘッダー + メッセージを送信バッファに格納
    send_buffer_.clear();
    send_buffer_.resize(MessageHeader::HEADER_SIZE + message.size());
    
    std::memcpy(send_buffer_.data(), &header, MessageHeader::HEADER_SIZE);
    std::memcpy(send_buffer_.data() + MessageHeader::HEADER_SIZE, message.data(), message.size());
    
    std::cout << "送信準備 - ID: " << network_to_host(header.message_id)
              << ", サイズ: " << message.size() << " バイト" << std::endl;
}

uint32_t HeaderConnection::host_to_network(uint32_t value) {
    return htonl(value);  // ホストからネットワークバイトオーダー
}

uint32_t HeaderConnection::network_to_host(uint32_t value) {
    return ntohl(value);  // ネットワークからホストバイトオーダー
}

// HeaderServer実装
HeaderServer::HeaderServer(boost::asio::io_context& io_context, short port)
    : ServerBase<HeaderConnection>(io_context, port) {
    std::cout << "固定長ヘッダーサーバー開始 - ポート: " << port << std::endl;
}

void HeaderServer::on_new_connection(std::shared_ptr<HeaderConnection> connection) {
    setup_connection_handlers(connection);
}

void HeaderServer::on_connection_error(const std::string& error) {
    std::cout << "サーバーエラー: " << error << std::endl;
}

void HeaderServer::setup_connection_handlers(std::shared_ptr<HeaderConnection> connection) {
    // メッセージ受信時の処理
    connection->set_message_handler([connection](const std::string& message) {
        // エコー応答を送信
        std::string response = "Echo: " + message;
        connection->send_message(response);
    });

    // エラー時の処理
    connection->set_error_handler([](const std::string& error) {
        std::cout << "接続エラー: " << error << std::endl;
    });
}