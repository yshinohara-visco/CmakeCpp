#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>

// AsyncWorkerをshared_ptrで使用するためのベース
class AsyncWorker : public std::enable_shared_from_this<AsyncWorker> {
public:
    AsyncWorker(boost::asio::io_context& io) : io_(io), work_count_(0) {}
    
    void start_async_work() {
        std::cout << "非同期作業を開始します..." << std::endl;
        
        for (int i = 1; i <= 5; ++i) {
            schedule_task(i);
        }
    }
    
private:
    void schedule_task(int task_id) {
        auto self = shared_from_this();
        io_.post([self, task_id]() {
            self->execute_task(task_id);
        });
    }
    
    void execute_task(int task_id) {
        std::cout << "タスク " << task_id << " を実行中... (スレッドID: " 
                  << std::this_thread::get_id() << ")" << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * task_id));
        
        work_count_++;
        std::cout << "タスク " << task_id << " 完了 (総完了数: " 
                  << work_count_ << ")" << std::endl;
    }
    
    boost::asio::io_context& io_;
    std::atomic<int> work_count_;
};

int main() {
    std::cout << "=== io_context 非同期処理サンプル ===" << std::endl;
    
    boost::asio::io_context io;
    
    // AsyncWorkerを作成
    auto worker = std::make_shared<AsyncWorker>(io);
    worker->start_async_work();
    
    // 複数スレッドでio_contextを実行
    std::vector<std::thread> threads;
    const int thread_count = 3;
    
    std::cout << "メインスレッドID: " << std::this_thread::get_id() << std::endl;
    std::cout << thread_count << "個のワーカースレッドを開始..." << std::endl;
    
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back([&io, i]() {
            std::cout << "ワーカースレッド " << i + 1 
                      << " 開始 (ID: " << std::this_thread::get_id() << ")" << std::endl;
            io.run();
            std::cout << "ワーカースレッド " << i + 1 << " 終了" << std::endl;
        });
    }
    
    // すべてのスレッドの終了を待機
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "すべての非同期処理が完了しました" << std::endl;
    
    return 0;
}