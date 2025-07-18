#include <boost/asio.hpp>
#include <iostream>

int main() {
    std::cout << "=== io_context 基本使用例 ===" << std::endl;
    
    // io_contextを作成
    boost::asio::io_context io;
    
    std::cout << "io_context作成完了" << std::endl;
    
    // work_guardを使用してio_contextが即座に終了しないようにする
    auto work_guard = boost::asio::make_work_guard(io);
    
    std::cout << "work_guard作成完了" << std::endl;
    
    // 作業をポストして実行
    io.post([]() {
        std::cout << "非同期タスク1が実行されました" << std::endl;
    });
    
    io.post([]() {
        std::cout << "非同期タスク2が実行されました" << std::endl;
    });
    
    io.post([]() {
        std::cout << "非同期タスク3が実行されました" << std::endl;
    });
    
    std::cout << "タスクをポスト完了、実行開始..." << std::endl;
    
    // 3つのタスクを実行
    std::size_t executed = io.run_one();
    std::cout << "実行されたタスク数: " << executed << std::endl;
    
    executed = io.run_one();
    std::cout << "実行されたタスク数: " << executed << std::endl;
    
    executed = io.run_one();
    std::cout << "実行されたタスク数: " << executed << std::endl;
    
    // work_guardを解除
    work_guard.reset();
    
    std::cout << "全てのタスクが完了しました" << std::endl;
    
    return 0;
}