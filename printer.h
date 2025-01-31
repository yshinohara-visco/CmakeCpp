#pragma once

#include <memory>

template <typename T>
class Test
{
protected: // 内部実装
    class Holder
    {
    public:
        virtual void set(T value) = 0;
        virtual void run() = 0;
    };

    // 型ごと共通の処理にしたい
    class PrinterA
    {
    public:
        virtual void print() = 0;
    };
    static std::shared_ptr<PrinterA> CreatePrinterA(T value);

    // 型ごとに異なる処理にしたい
    class PrinterB
    {
    public:
        virtual void print() = 0;
    };
    static std::shared_ptr<PrinterB> CreatePrinterB(T value);

public: // 外部インターフェース
    static auto CreateHolder(T value) -> std::shared_ptr<Holder>;
};

using TestInt = Test<int>;
using TestDouble = Test<double>;