#include "printer.h"

#include <iostream>

template <typename T>
class TestImpl : public Test<T>
{
public:
    class HolderImpl : public Test<T>::Holder
    {
    public:
        HolderImpl(T value)
        {
            m_value = value;
        }
        void set(T value) override
        {
            m_value = value;
        }
        void run() override
        {
            auto printerA = CreatePrinterA(m_value);
            printerA->print();

            auto printerB = CreatePrinterB(m_value);
            printerB->print();
        }
    private:
        T m_value;
    };
    static std::shared_ptr<Holder> CreateHolder(T value)
    {
        return std::make_shared<HolderImpl>( value );
    }

protected:
    // 実体化したint,doubleで共通
    class PrinterAImpl : public Test<T>::PrinterA
    {
    public:
        PrinterAImpl(T value)
        {
            m_value = value;
        }
        void print() override
        {
            std::cout << "PrinterAImpl" << m_value << std::endl;
        }
    private:
        T m_value;
    };
    static std::shared_ptr<PrinterA> CreatePrinterA(T value)
    {
        return std::make_shared<PrinterAImpl>(value);
    }

    // intで使用され、特殊化したdoubleでは使用されない
    class PrinterBImpl : public Test<T>::PrinterB
    {
    public:
        PrinterBImpl(T value)
        {
            m_value = value;
        }
        void print() override
        {
            std::cout << "PrinterBImpl" << m_value << std::endl;
        }
    private:
        T m_value;
    };

    static std::shared_ptr<PrinterB> CreatePrinterB(T value)
    {
        return std::make_shared<PrinterBImpl>(value);
    }
};

// PrinterBの特殊化
template<>
class TestImpl<double>::PrinterBImpl : public Test<double>::PrinterB
{
public:
    PrinterBImpl(double value)
    {
        m_value = value;
    }
    void print() override
    {
        std::cout << "PrinterBImpl double: " << m_value << std::endl;
    }
private:
    double m_value;
};


// 外部から利用する処理の実体化 (「Test<T>の」CreateHolder)
template <typename T>
auto Test<T>::CreateHolder(T value) -> std::shared_ptr<Holder>
{
    return TestImpl<T>::CreateHolder(value);
}
template auto Test<int>::CreateHolder(int value) -> std::shared_ptr<Holder>;
template auto Test<double>::CreateHolder(double value) -> std::shared_ptr<Holder>;