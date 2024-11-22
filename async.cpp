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


    // async��future��Ԃ��Afuture��wait���Ăяo�����ƂŌ��ʂ��擾�ł���
    auto f = std::async(std::launch::async, [&]{heavyPrint("Hello from async!!");});
    print("Hello from main!"); // �����炪��ɕ\�������
    f.wait();

    print("------------------");

    // std::launch::async�ŋN������future���󂯎��Ȃ���΁A���̏��wait����炵��(�񓯊��ɂȂ�Ȃ�)
    std::async(std::launch::async, [&]{heavyPrint("Hello from async!!");});
    print("Hello from main!"); // �����炪��ɕ\�������

    print("------------------");
}

void TestPromise()
{
    // ���promise���쐬���A����future���擾����
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t([&p]{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // promise�ɒl��ݒ肷��
        p.set_value(42);
    });

    // future��get���Ăяo�����Ƃ�promise�̒l���擾����
    std::cout << "The answer is " << f.get() << std::endl;

    std::cout << "------------------\n";
    t.join();
}

// async�̕���promise�����Ȍ��ɏ�����?
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

// promise,future���g���΁A�u���ʂ�ݒ肷���v�A�u�Q�Ƃ����v�����₷���H
// async��Ԃ��֐���������future��Consumer�ɓn�������Y�킩�H
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

    std::thread t1(Producer, std::ref(p)); // ���ʏo���z1
    std::thread t2(Consumer, std::ref(f)); // �󂯎����

    t1.join();
    t2.join();

    std::cout << "------------------\n";

    std::promise<int> p2;
    std::future<int> f2 = p2.get_future();

    std::thread t3(Producer2, std::ref(p2)); // ���ʏo���z2
    std::thread t4(Consumer, std::ref(f2)); // �󂯎���͓���

    t3.join();
    t4.join();

    std::cout << "------------------\n";
}