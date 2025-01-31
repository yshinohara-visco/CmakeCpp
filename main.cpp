#include "template.h"
#include "printer.h"

int main()
{
    {
        Template1<int> t1(1);
        t1.Print();
        Template1<double> t2(3.14);
        t2.Print();
    }

    {
        Template2<int> t1(2);
        t1.Print();
        // Template2<double> t4(2.71); // 実体化していないのでコンパイルエラー
    }

    {
        Template3<int> t1(3);
        t1.Print();
        Template3<double> t2(2.71);
        t2.Print();
    }

    {
        Template4<int>::Inner t1(4);
        t1.Print();
    }

    {
        auto t5 = CreateTemplate5<int>(5);
        t5->Print();
        delete t5;
    }

    {
        auto t6 = Template6<int>::CreateInner(6);
        t6->Print();
        delete t6;
    }

    {
        auto t6 = Template6<double>::CreateInner(6.28);
        t6->Print();
        delete t6;
    }

    {
        auto holder = TestInt::CreateHolder(7);
        holder->set(7);
        holder->run();

        auto holder2 = TestDouble::CreateHolder(7.77);
        holder2->set(7.77);
        holder2->run();
    }
    
	return 0;
}
