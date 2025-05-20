#include <iostream>

#include "di.h"
#include "boost/di.h"

int main()
{
	{
		Hoge hoge;
		hoge.SayHoge();

		A a(hoge);
		a.Say();

		B b(a);
		b.Say();
	}
	std::cout << "----------------" << std::endl;

	namespace di = boost::di;
	{
		//引数コンストラクタのみを要求する場合、勝手に作るらしい
		auto injector = di::make_injector();
		auto b = injector.create<B>();
		b.Say();
	}

	{
		auto injector = di::make_injector(
		    di::bind<Hoge>().to<Hoge>(),
			di::bind<std::string>().to("str"),
			di::bind<A2>().to<A2>(),
			di::bind<B2>().to<B2>()
		);
		auto b = injector.create<B2>();
		b.Say();
	}

	// 一度に初期化できる場合はともかく、途中で色々処理した結果を受けて初期化する場合にどうするか良く分からない
	// 複数作成する場合とかもどうするか分からない
	

	return 0;
}
