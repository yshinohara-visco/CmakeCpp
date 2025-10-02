#include <iostream>
#include "origin/engine.h"
#include "di/engine.h"
#include "di/di_module.h"

#include <boost/version.hpp>

int main()
{
	{
		std::cout << "Boost version: " << BOOST_VERSION / 100000 << "."
			<< BOOST_VERSION / 100 % 1000 << "."
			<< BOOST_VERSION % 100 << std::endl;
	}

	std::cout << "\n=== Running Original Version ===" << std::endl;
	{
		origin::Engine engine;
		engine.run("output_origin.txt");
		std::cout << "Original version completed!" << std::endl;
	}

	std::cout << "\n=== Running DI Version ===" << std::endl;
	{
		// DIコンテナを作成
		auto injector = di::createModule();

		// EngineをDIコンテナから取得（依存関係は自動的に解決される）
		auto engine = injector.create<std::shared_ptr<di::Engine>>();

		engine->run("output_di.txt");
		std::cout << "DI version completed!" << std::endl;
	}

	return 0;
}
