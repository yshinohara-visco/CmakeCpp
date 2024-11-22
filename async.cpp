#include "async.h"

#include <iostream>
#include <future>
#include <functional>

void TestAsync()
{
    std::mutex mtx_print;
    auto print = [&mtx_print](const std::string& msg) {
        std::lock_guard<std::mutex> lock(mtx_print);
        std::cout << msg << std::endl;
    };

    auto heavyPrint = [&](std::string str) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        print(str);
        };


    // asyncはfutureを返し、futureはwaitを呼び出すことで結果を取得できる
    auto f = std::async(std::launch::async, [&]{heavyPrint("Hello from async!!");});
    print("Hello from main!"); // こちらが先に表示される
    f.wait();

    print("------------------");

    // std::launch::asyncで起動したfutureを受け取らなければ、その場でwaitするらしい(非同期にならない)
    std::async(std::launch::async, [&]{heavyPrint("Hello from async!!");});
    print("Hello from main!"); // こちらが後に表示される

    print("------------------");
}

void TestPromise()
{
    // 先にpromiseを作成し、そのfutureを取得する
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t([&p]{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // promiseに値を設定する
        p.set_value(42);
    });

    // futureのgetを呼び出すことでpromiseの値を取得する
    std::cout << "The answer is " << f.get() << std::endl;

    std::cout << "------------------\n";
    t.join();
}

// asyncの方がpromiseよりも簡潔に書ける?
auto CalcAsync(int a, int b ) -> std::future<int>
{
    return std::async(std::launch::async, [a, b]{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return a + b;
    });
}
auto CalcPromise(int a, int b) -> std::future<int>
{
    std::promise<int> p;
    auto f = p.get_future();

    std::thread t([&p, a, b]{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        p.set_value(a + b);
    });

    return f;
}

// promise,futureを使えば、「結果を設定するやつ」、「参照するやつ」を作りやすい？
// asyncを返す関数から貰ったfutureをConsumerに渡す方が綺麗か？
void Producer(std::promise<int>& p)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    p.set_value(42);
}
void Producer2(std::promise<int>& p)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    p.set_value(84);
}
void Consumer(std::future<int>& f)
{
    std::cout << "The answer is " << f.get() << std::endl;
}
void TestProducerConsumer()
{
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t1(Producer, std::ref(p)); // 結果出す奴1
    std::thread t2(Consumer, std::ref(f)); // 受け取るやつ

    t1.join();
    t2.join();

    std::cout << "------------------\n";

    std::promise<int> p2;
    std::future<int> f2 = p2.get_future();

    std::thread t3(Producer2, std::ref(p2)); // 結果出す奴2
    std::thread t4(Consumer, std::ref(f2)); // 受け取るやつは同じ

    t3.join();
    t4.join();

    std::cout << "------------------\n";
}