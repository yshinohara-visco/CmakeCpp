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
    // ���̉�����int,double�ŋ���
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

    // int�Ŏg�p����A���ꉻ����double�ł͎g�p����Ȃ�
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

// PrinterB�̓��ꉻ
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


// �O�����痘�p���鏈���̎��̉� (�uTest<T>�́vCreateHolder)
template <typename T>
auto Test<T>::CreateHolder(T value) -> std::shared_ptr<Holder>
{
    return TestImpl<T>::CreateHolder(value);
}
template auto Test<int>::CreateHolder(int value) -> std::shared_ptr<Holder>;
template auto Test<double>::CreateHolder(double value) -> std::shared_ptr<Holder>;