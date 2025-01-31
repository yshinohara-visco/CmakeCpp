#pragma once

#include <memory>

template <typename T>
class Test
{
protected: // ��������
    class Holder
    {
    public:
        virtual void set(T value) = 0;
        virtual void run() = 0;
    };

    // �^���Ƌ��ʂ̏����ɂ�����
    class PrinterA
    {
    public:
        virtual void print() = 0;
    };
    static std::shared_ptr<PrinterA> CreatePrinterA(T value);

    // �^���ƂɈقȂ鏈���ɂ�����
    class PrinterB
    {
    public:
        virtual void print() = 0;
    };
    static std::shared_ptr<PrinterB> CreatePrinterB(T value);

public: // �O���C���^�[�t�F�[�X
    static auto CreateHolder(T value) -> std::shared_ptr<Holder>;
};

using TestInt = Test<int>;
using TestDouble = Test<double>;