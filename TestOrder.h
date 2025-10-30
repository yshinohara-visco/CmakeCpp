#pragma once

#include "my_class.h"

namespace TestOrder
{

/*
メンバ変数の初期化の順序は、宣言された順序によって決まる。
メンバ変数の解放の順序は、初期化の逆順になる。
初期化リストの順序は関係ない。
出力結果:
Construct: a
Construct: b
Construct: c
Test1 Constructor
Test1 Destructor
Destruct: c
Destruct: b
Destruct: a
*/
class Test1
{
public:
    Test1()
        : c_("c"), b_("b"), a_("a")
    {
        std::cout << "Test1 Constructor" << std::endl;
    }
    ~Test1()
    {
        std::cout << "Test1 Destructor" << std::endl;
    }

private:
    MyClass a_;
    MyClass b_;
    MyClass c_;
};


/*
継承した時、親クラスのメンバ変数、親クラスのコンストラクタ、子クラスのメンバ変数、子クラスのコンストラクタの順で初期化される。
解放はその逆順になる。

出力結果：
Construct: a
Construct: b
Construct: c
Test1 Constructor
Construct: d
Test2 Constructor
Test2 Destructor
Destruct: d
Test1 Destructor
Destruct: c
Destruct: b
Destruct: a
*/
class Test2 : public Test1
{
public:
    Test2()
        : d_("d")
    {
        std::cout << "Test2 Constructor" << std::endl;
    }
    ~Test2()
    {
        std::cout << "Test2 Destructor" << std::endl;
    };
private:
    MyClass d_;
};

} // namespace TestOrder