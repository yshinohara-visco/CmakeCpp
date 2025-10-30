
#include "TestOrder.h"
#include "TestAsync.h"

int main()
{
    {
        TestOrder::Test1 test1;
    }
    std::cout << "-------------------" << std::endl;
    {
        TestOrder::Test2 test2;
    }
    std::cout << "-------------------" << std::endl;
    {
        TestAsync::TestAsync1 testAsync1;
    }
    std::cout << "-------------------" << std::endl;
    {
        TestAsync::TestAsync2 testAsync2;
    }
    std::cout << "-------------------" << std::endl;
    {
        TestAsync::TestAsync3 testAsync3;
    }

	return 0;
}
