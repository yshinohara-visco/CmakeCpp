#include <iostream>
#include <memory>
#include <crtdbg.h>


class InterfaceA {
public:
    virtual void methodA() = 0;
};

class InterfaceB {
public:
    virtual void methodB() = 0;
};

class InterfaceX {
public:
    virtual void methodX() = 0;
};

class ClassC : public InterfaceA, public InterfaceB {
public:
    ClassC()
        : data_(0)
    {
        std::cout << "ClassC constructed. data = " << data_ << std::endl;
    }

    virtual ~ClassC()
    {
        std::cout << "ClassC destructed. data = " << data_ << std::endl;
    }

    void Init()
    {
        data_ = 0;
        std::cout << "ClassC::Init" << std::endl;
    }

    void methodA() override
    {
        data_++;
        std::cout << "ClassC::methodA. data = " << data_ << std::endl;
    }

    void methodB() override
    {
        data_++;
        std::cout << "ClassC::methodB. data = " << data_ << std::endl;
    }

private:
    int data_;
};

int main()
{
    // メモリリーク検出機能を有効にする
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    {
        // cInstanceの作成
       auto cInstance = std::make_shared<ClassC>();
       cInstance->Init();
       std::cout << "main: cInstance use_count = " << cInstance.use_count() << std::endl;

       // InterfaceAのインスタンスとして使用
       std::shared_ptr<InterfaceA> a = cInstance;
       std::cout << "main: a use_count = " << a.use_count() << std::endl;
       a->methodA();

       // a->bにキャストして使用
       if(auto b = std::dynamic_pointer_cast<InterfaceB>(a))
       {
            std::cout << "main: b use_count = " << b.use_count() << std::endl;
            b->methodB();
       }
       else
       {
            std::cout << "main: b is nullptr" << std::endl;
       }

       //誤ったキャストはnullptrを返す
       if(auto x = std::dynamic_pointer_cast<InterfaceX>(a)) 
       {
            std::cout << "main: x use_count = " << x.use_count() << std::endl;
            x->methodX();
       }
       else
       {
            std::cout << "main: x is nullptr" << std::endl;
       }
    }

    // メモリリークの検出を強制
    _CrtDumpMemoryLeaks();
    return 0;
}
