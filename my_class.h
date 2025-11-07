#pragma once

#include <memory>

// Holderに所有権を持たれたまま、他の処理からも参照したい
class Printer
{
public:
    Printer();
    ~Printer();
    void Print(); // ReadOnly
    void Print(int num); // Write
    void SetNum(int num); // Write
    int GetNum(); // ReadOnly

private:
    int num_;
};

// Printerの所有権を持つ唯一の存在でありたい
class Holder
{
public:
    Holder();
    ~Holder();

    void Launch1();
    void Launch2();
    void Launch2b();
    void Launch3();
    void Launch4();

private:
    Printer b_;
    Printer* pb_;
    std::unique_ptr<Printer> upb_;
    std::shared_ptr<Printer> spb_;
};
