#pragma once

#include <iostream>
#include <string>

class Hoge
{
public:
    void SayHoge() { std::cout << "Hoge" << std::endl; }
};

class A
{
public:
    A(Hoge& hoge) : hoge_(hoge) {}
    void Say()
    {
        std::cout << "A" << std::endl;
        hoge_.SayHoge();
    }

private:
    Hoge& hoge_;
};


class B
{
public:
    B(A& a) : a_(a) {}
    void Say()
    {
        std::cout << "B" << std::endl;
        a_.Say();
    }
private:
    A& a_;
};

class A2
{
public:
    A2(Hoge& hoge, std::string str) : hoge_(hoge), str_(str) {}
    void Say()
    {
        std::cout << "A2 " << str_ << std::endl;
        hoge_.SayHoge();
    }
private:
    Hoge& hoge_;
    std::string str_;
};

class B2
{
public:
    B2(A2& a, std::string str) : a_(a), str_(str) {}
    void Say()
    {
        std::cout << "B2 " << str_ << std::endl;
        a_.Say();
    }
private:
    A2& a_;
    std::string str_;
};