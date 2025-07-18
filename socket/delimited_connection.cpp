#include "delimited_connection.hpp"
#include <iostream>
#include <sstream>

DelimitedConnection::DelimitedConnection(tcp::socket socket, char delimiter)
    : ConnectionBase(std::move(socket)), delimiter_(delimiter) {
}

void DelimitedConnection::start_receive() {
    if (!connected_) return;

    auto self = std::static_pointer_cast<DelimitedConnection>(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(receive_buffer_),
        [self](std::error_code ec, std::size_t bytes_received) {
            self->handle_receive(ec, bytes_received);
        });
}

void DelimitedConnection::handle_receive(const boost::system::error_code& ec, std::size_t bytes_received) {
    if (!ec && bytes_received > 0) {
        // 受信データを未完成メッセージに追加
        incomplete_message_.append(receive_buffer_.data(), bytes_received);
        process_received_data();
        start_receive(); // 次の受信を開始
    } else {
        if (ec == boost::asio::error::eof) {
            std::cout << "クライアント切断: " << get_remote_endpoint() << std::endl;
        } else {
            handle_error("受信エラー: " + ec.message());
        }
    }
}

void DelimitedConnection::process_received_data() {
    size_t pos;
    while ((pos = incomplete_message_.find(delimiter_)) != std::string::npos) {
        // 完成したメッセージを抽出
        std::string complete_message = incomplete_message_.substr(0, pos);
        incomplete_message_.erase(0, pos + 1); // 区切り文字も削除

        std::cout << "受信メッセージ: " << complete_message << std::endl;
        
        // メッセージハンドラーを呼び出し
        if (message_handler_) {
            message_handler_(complete_message);
        }
    }
}

void DelimitedConnection::prepare_send_data(const std::string& message) {
    // メッセージに区切り文字を追加
    send_buffer_ = message + delimiter_;
    std::cout << "送信準備: " << message << std::endl;
}

// DelimitedServer実装
DelimitedServer::DelimitedServer(boost::asio::io_context& io_context, short port, char delimiter)
    : ServerBase<DelimitedConnection>(io_context, port), delimiter_(delimiter) {
    std::cout << "区切り文字サーバー開始 - ポート: " << port 
              << ", 区切り文字: '" << delimiter_ << "'" << std::endl;
}

void DelimitedServer::on_new_connection(std::shared_ptr<DelimitedConnection> connection) {
    setup_connection_handlers(connection);
}

void DelimitedServer::on_connection_error(const std::string& error) {
    std::cout << "サーバーエラー: " << error << std::endl;
}

void DelimitedServer::setup_connection_handlers(std::shared_ptr<DelimitedConnection> connection) {
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