// #importは名前付きモジュール内では行わない
// グローバルモジュールフラグメントで行うらしい
module;
#include <iostream>

export module myModule;

export void hello() {
	std::cout << "Hello, World!" << std::endl;
}
