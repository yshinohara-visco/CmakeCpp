#pragma once

#include <iostream>

// �e���v���[�g�̎������w�b�_�ɋL�q����ꍇ
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

// cpp�Ŏ����A���̉�����ꍇ
template <typename T>
class Template2
{
public:
    Template2(T value);
    void Print();

private:
    T m_value;
};

// cpp�Ŏ��̉��A���ꉻ����ꍇ
template <typename T>
class Template3
{
public:
    Template3(T value);
    void Print(); 
private:
    T m_value;
};

// �e���v���[�g�N���X���N���X
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

// �e���v���[�g���z�N���X
template <typename T>
class Template5
{
public:
    virtual void Print() = 0;
};
template <typename T>
Template5<T>* CreateTemplate5(T value);


// �e���v���[�g�N���X�����z�N���X��cpp�œ��ꉻ
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

// �w�b�_�œ��ꉻ��cpp�Ŏ���
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