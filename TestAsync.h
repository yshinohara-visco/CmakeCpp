#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <memory>

#include "my_class.h"

/*
非同期処理の停止をデストラクタで行うと、動きっぱなしにならないという面では安全。
デストラクタで止まるし、それに任せてコードを減らしたくなる。

しかし、非同期処理がクラス外のオブジェクトを参照する場合は危険がある。
非同期処理が後に定義されたオブジェクトを参照している場合、非同期処理の停止前にそのオブジェクトが解放されてしまう可能性がある。
非同期処理は参照先が確実に存在するうちに明示的に停止させるべき。
*/

namespace TestAsync
{

// 文字列を保持して出力するクラス
class Printer
{
public:
    Printer(std::string str)
        : str_(str)
    {
        std::cout << "Printer Constructor: " << str_ << std::endl;
    }
    ~Printer()
    {
        std::cout << "Printer Destructor: " << str_ << std::endl;
    }

    void Print() const
    {
        std::cout << "Print: " << str_ << std::endl;
    }

private:
    std::string str_;
};

// 非同期でPrinterのPrintメソッドを定期的に呼び出すクラス
// Printerの所有権は持たず、別のところにあるPrinterを使用する
class AsyncPrinter
{
public:
    AsyncPrinter(Printer* printer)
        : printer_(printer)
    {
        std::cout << "AsyncPrinter Constructor" << std::endl;
        thread_ = std::thread(&AsyncPrinter::ThreadFunc, this);
    }
    ~AsyncPrinter()
    {
        std::cout << "AsyncPrinter Destructor" << std::endl;
        stop_ = true;
        if (thread_.joinable())
        {
            thread_.join();
        }
    }
    void Stop()
    {
        stop_ = true;
        if (thread_.joinable())
        {
            thread_.join();
        }
    }

private:
    void ThreadFunc()
    {
        while (!stop_)
        {
            printer_->Print();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    Printer* printer_;
    std::thread thread_;
    std::atomic_bool stop_{ false };
};

// 解放に1秒かかるクラス
class DelayedDestructor
{
public:
    DelayedDestructor()
    {
        std::cout << "DelayedDestructor Constructor" << std::endl;
    }
    ~DelayedDestructor()
    {
        std::cout << "DelayedDestructor Destructor: Before sleep" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "DelayedDestructor Destructor: After sleep" << std::endl;
    }
};

/*
AsyncPrinterの使用例1
メンバ変数をPrinter, AsyncPrinterの順に配置している

出力結果：
Printer Constructor: Async1 Printer
DelayedDestructor Constructor
AsyncPrinter Constructor
TestAsync1 Constructor
Print: Async1 Printer
Print: Async1 Printer
Print: Async1 Printer
TestAsync1 Destructor
AsyncPrinter Destructor
DelayedDestructor Destructor: Before sleep
DelayedDestructor Destructor: After sleep
Printer Destructor: Async1 Printer

AsyncPrinterのデストラクタが先に呼ばれ、Printerの解放前にスレッドが停止しているため、安全に動作している。
メンバ変数の定義順に依存しているので危険ではある。誰かが変えるかもしれない。
*/
class TestAsync1
{
public:
    TestAsync1()
        : printer_("Async1 Printer"), async_printer_(&printer_)
    {
        std::cout << "TestAsync1 Constructor" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    ~TestAsync1()
    {
        std::cout << "TestAsync1 Destructor" << std::endl;
    }

private:
    Printer printer_;
    DelayedDestructor d_;
    AsyncPrinter async_printer_;
};

// 
/*
AsyncPrinterの使用例2
メンバ変数をAsyncPrinter, Printerの順に配置している

出力結果：
AsyncPrinter Constructor
DelayedDestructor Constructor
Print: Async2 Printer
Printer Constructor: Async2 Printer
TestAsync2 Constructor
Print: Async2 Printer
Print: Async2 Printer
TestAsync2 Destructor
Printer Destructor: Async2 Printer
DelayedDestructor Destructor: Before sleep
Print:
Print:
Print:
Print:
Print:
Print:
Print:
Print:
Print:
Print:
DelayedDestructor Destructor: After sleep
AsyncPrinter Destructor

明らかに解放済みのPrinterにアクセスしている。
落ちていないのはメモリ上に半端に残っているからだと思われる。
未定義動作であり、非常に危険なコードである。
下手に動く分、環境やタイミングによっては落ちる発見しづらいバグになる可能性がある。
*/
class TestAsync2
{
public:
    TestAsync2()
        : printer_("Async2 Printer"), async_printer_(&printer_)
    {
        std::cout << "TestAsync2 Constructor" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    ~TestAsync2()
    {
        std::cout << "TestAsync2 Destructor" << std::endl;
    }
private:
    AsyncPrinter async_printer_;
    DelayedDestructor d_;
    Printer printer_;
};


/*
AsyncPrinterの使用例3
Printerの解放前にAsyncPrinterを明示的に停止している。


出力結果：
AsyncPrinter Constructor
DelayedDestructor Constructor
Printer Constructor: Async3 Printer
Print: Async3 Printer
TestAsync3 Constructor
Print: Async3 Printer
Print: Async3 Printer
TestAsync3 Destructor
Printer Destructor: Async3 Printer
DelayedDestructor Destructor: Before sleep
DelayedDestructor Destructor: After sleep
AsyncPrinter Destructor

明示敵に停止しているため、安全に動作している。
非同期処理を使う場合は、このように明示的に停止する方法を取るべきである。
*/
class TestAsync3
{
public:
    TestAsync3()
        : printer_("Async3 Printer"), async_printer_(&printer_)
    {
        std::cout << "TestAsync3 Constructor" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    ~TestAsync3()
    {
        std::cout << "TestAsync3 Destructor" << std::endl;
        async_printer_.Stop(); // 明示的に停止
    }
private:
    AsyncPrinter async_printer_;
    DelayedDestructor d_;
    Printer printer_;
};

}