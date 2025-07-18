#include <boost/asio.hpp>
#include <iostream>
#include <chrono>

int main() {
    std::cout << "=== io_context タイマーサンプル ===" << std::endl;
    
    boost::asio::io_context io;
    
    // steady_timerを作成（2秒後に実行）
    boost::asio::steady_timer timer1(io, std::chrono::seconds(2));
    
    // 非同期タイマー処理
    timer1.async_wait([](const boost::system::error_code& ec) {
        if (!ec) {
            std::cout << "タイマー1: 2秒経過しました" << std::endl;
        } else {
            std::cout << "タイマー1エラー: " << ec.message() << std::endl;
        }
    });
    
    // 別のタイマー（1秒後に実行）
    boost::asio::steady_timer timer2(io, std::chrono::seconds(1));
    
    timer2.async_wait([](const boost::system::error_code& ec) {
        if (!ec) {
            std::cout << "タイマー2: 1秒経過しました" << std::endl;
        } else {
            std::cout << "タイマー2エラー: " << ec.message() << std::endl;
        }
    });
    
    // 連続タイマー（0.5秒間隔で3回実行）
    auto counter = std::make_shared<int>(0);
    boost::asio::steady_timer timer3(io, std::chrono::milliseconds(500));
    
    std::function<void(const boost::system::error_code&)> repeat_timer = 
        [&io, &timer3, counter, &repeat_timer](const boost::system::error_code& ec) {
            if (!ec) {
                (*counter)++;
                std::cout << "連続タイマー: " << *counter << "回目の実行" << std::endl;
                
                if (*counter < 3) {
                    timer3.expires_after(std::chrono::milliseconds(500));
                    timer3.async_wait(repeat_timer);
                }
            }
        };
    
    timer3.async_wait(repeat_timer);
    
    std::cout << "タイマー開始、実行を待機中..." << std::endl;
    
    // すべての非同期操作を実行
    io.run();
    
    std::cout << "すべてのタイマーが完了しました" << std::endl;
    
    return 0;
}