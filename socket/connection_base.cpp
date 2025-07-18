#include "connection_base.hpp"
#include <iostream>

ConnectionBase::ConnectionBase(tcp::socket socket)
    : socket_(std::move(socket)), connected_(true) {
}

void ConnectionBase::start() {
    try {
        std::cout << "新しい接続開始: " << get_remote_endpoint() << std::endl;
        on_start();
    } catch (const std::exception& e) {
        handle_error("接続開始エラー: " + std::string(e.what()));
    }
}

void ConnectionBase::send_message(const std::string& message) {
    if (!connected_) {
        handle_error("切断された接続に送信しようとしました");
        return;
    }

    auto self = shared_from_this();
    boost::asio::post(socket_.get_executor(), [self, message]() {
        bool write_in_progress = !self->send_buffer_.empty();
        self->prepare_send_data(message);
        if (!write_in_progress) {
            self->start_send();
        }
    });
}

void ConnectionBase::disconnect() {
    if (connected_) {
        std::cout << "接続を手動で切断: " << get_remote_endpoint() << std::endl;
        close_connection();
    }
}

tcp::endpoint ConnectionBase::get_remote_endpoint() const {
    try {
        return socket_.remote_endpoint();
    } catch (const std::exception&) {
        return tcp::endpoint();  // 無効なエンドポイント
    }
}

void ConnectionBase::start_send() {
    if (!connected_ || send_buffer_.empty()) {
        return;
    }

    auto self = shared_from_this();
    boost::asio::async_write(socket_, boost::asio::buffer(send_buffer_),
        [self](std::error_code ec, std::size_t bytes_sent) {
            self->handle_send(ec, bytes_sent);
        });
}

void ConnectionBase::handle_send(const boost::system::error_code& ec, std::size_t bytes_sent) {
    if (!ec) {
        std::cout << "送信完了: " << bytes_sent << " バイト" << std::endl;
        send_buffer_.clear();
        // 送信キューに他のメッセージがあれば続けて送信
    } else {
        handle_error("送信エラー: " + ec.message());
    }
}

void ConnectionBase::handle_error(const std::string& error_msg) {
    std::cout << "エラー: " << error_msg << std::endl;
    if (error_handler_) {
        error_handler_(error_msg);
    }
    close_connection();
}

void ConnectionBase::close_connection() {
    if (connected_) {
        connected_ = false;
        boost::system::error_code ec;
        socket_.close(ec);
        std::cout << "接続終了" << std::endl;
    }
}

void ConnectionBase::on_start() {
    start_receive();
}