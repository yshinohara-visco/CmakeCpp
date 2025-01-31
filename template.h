#pragma once

#include <iostream>

// テンプレートの実装をヘッダに記述する場合
template <typename T>
class Template1
{
public:
    Template1(T value)
    {
       m_value = value;
    }

    void Print()
    {
        std::cout << "Template1: " << m_value << std::endl;
    }

private:
    T m_value;
};

// cppで実装、実体化する場合
template <typename T>
class Template2
{
public:
    Template2(T value);
    void Print();

private:
    T m_value;
};

// cppで実体化、特殊化する場合
template <typename T>
class Template3
{
public:
    Template3(T value);
    void Print(); 
private:
    T m_value;
};

// テンプレートクラス内クラス
template <typename T>
class Template4
{
public:
    class Inner
    {
    public:
        Inner(T value);
        void Print();
    private:
        T m_value;
    };
};

// テンプレート仮想クラス
template <typename T>
class Template5
{
public:
    virtual void Print() = 0;
};
template <typename T>
Template5<T>* CreateTemplate5(T value);


// テンプレートクラス内仮想クラス→cppで特殊化
template <typename T>
class Template6
{
protected:
    class Inner
    {
    public:
        virtual void Print() = 0;
    };
public:
    static Inner* CreateInner(T value);
};

// ヘッダで特殊化→cppで実装
template <>
class Template6<double>
{
protected:
    class Inner
    {
    public:
        Inner(double value);
        void Print();
    private:
        double m_value;
    };
public:
    static Inner* CreateInner(double value);
};