#include "my_class.h"

#include <iostream>
#include <thread>

Holder::Holder()
    : pb_(new Printer()),
    upb_(std::make_unique<Printer>()),
    spb_(std::make_shared<Printer>())
{
    b_.SetNum(1);
    pb_->SetNum(2);
    upb_->SetNum(3);
    spb_->SetNum(4);
}

Holder::~Holder()
{
    delete pb_;

    // これをするかでLaunch2の挙動が変わる。
    // nullptrを代入すると、Launch2b内でnullptrチェックが働く。
    // 生ポインタを使うのも解放時にnullptrを代入するのを前提にするのも危険。
    // pb_ = nullptr;

    // これをするかでLaunch3の挙動が変わる。
    // unique_ptrを解放すると、Launch3内でnullptrチェックが働く。
    // デストラクタで同じ操作をしているものと思ったが、コメントアウトすると働かない。
    // upb_.reset();


    // そもそも参照しているpb_などが解放されているので、明示的に解放とかそういう問題では無かった。
}

void Holder::Launch1()
{
    std::thread t1([&b = b_]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        b.SetNum(11);
        b.Print(11);
        });
    t1.detach();
}

void Holder::Launch2()
{
    // ポインタをコピーした場合
    std::thread t2([pb = pb_]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (pb)
            pb->Print(22);
        else
            std::cout << "pb_ is nullptr." << std::endl;
        });
    t2.detach();
}

void Holder::Launch2b()
{
    // ポインタの参照を渡した場合
    std::thread t2_ref([&pb = pb_]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (pb)
            pb->Print(222);
        else
            std::cout << "pb_ is nullptr." << std::endl;
        });
    t2_ref.detach();
}

// unique_ptrの参照を渡した場合、それが解放されたか判断できるのか？
void Holder::Launch3()
{
    std::thread t3([&upb = upb_]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (upb)
            upb->Print(33);
        else
            std::cout << "upb_ has been released." << std::endl;
        });
    t3.detach();
}

void Holder::Launch4()
{
    std::thread t4([wpb = std::weak_ptr(spb_)]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (auto spb = wpb.lock())
            spb->Print(33);
        else
            std::cout << "spb_ has been released." << std::endl;
        });
    t4.detach();
}

Printer::Printer()
    : num_(0)
{
}

Printer::~Printer()
{
}

void Printer::Print()
{
    std::cout << "num_: " << num_ << std::endl;
}

void Printer::Print(int num)
{
    num_ = num;
    std::cout << "num_: " << num_ << std::endl;
}

void Printer::SetNum(int num)
{
    num_ = num;
}

int Printer::GetNum()
{
    return num_;
}