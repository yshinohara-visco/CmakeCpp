#include "template.h"

template <typename T>
Template2<T>::Template2(T value)
{
    m_value = value;
}

//---------------------------------------------------------------------

template <typename T>
void Template2<T>::Print()
{
    std::cout << "Template2: " << m_value << std::endl;
}

// ŽÀ‘Ì‰»
template Template2<int>::Template2(int value);
template void Template2<int>::Print();

//---------------------------------------------------------------------

// ŽÀ‘Ì‰»
template <typename T>
Template3<T>::Template3(T value)
{
    m_value = value;
}
template Template3<int>::Template3(int value);
template Template3<double>::Template3(double value);

// “ÁŽê‰»
template <>
void Template3<int>::Print()
{
    std::cout << "Template3 int: " << m_value << std::endl;
}
template<>
void Template3<double>::Print()
{
    std::cout << "Template3 double: " << m_value << std::endl;
}

//---------------------------------------------------------------------

template <typename T>
Template4<T>::Inner::Inner(T value)
{
    m_value = value;
}

template <typename T>
void Template4<T>::Inner::Print()
{
    std::cout << "Template4 Inner: " << m_value << std::endl;
}

// ŽÀ‘Ì‰»
template Template4<int>::Inner::Inner(int value);
template void Template4<int>::Inner::Print();

//---------------------------------------------------------------------

class Template5Impl : public Template5<int>
{
public:
    Template5Impl(int value)
    {
        m_value = value;
    }
    void Print()
    {
        std::cout << "Template5: " << m_value << std::endl;
    }
private:
    int m_value;
};

template <>
Template5<int>* CreateTemplate5(int value)
{
    return new Template5Impl(value);
}

//---------------------------------------------------------------------

class Template6Impl : public Template6<int>
{
public:
    class InnerImpl : public Inner
    {
    public:
        InnerImpl(int value)
        {
            m_value = value;
        }
        void Print()
        {
            std::cout << "Template6 Inner: " << m_value << std::endl;
        }
     private:
        int m_value;
    };
};

template <>
Template6<int>::Inner* Template6<int>::CreateInner(int value)
{
    return new Template6Impl::InnerImpl(value);
}

//---------------------------------------------------------------------

Template6<double>::Inner::Inner(double value)
{
    m_value = value;
}
void Template6<double>::Inner::Print()
{
    std::cout << "Template6 double: " << m_value << std::endl;
}
Template6<double>::Inner* Template6<double>::CreateInner(double value)
{
    return new Template6<double>::Inner(value);
}