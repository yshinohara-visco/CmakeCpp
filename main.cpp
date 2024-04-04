#include <iostream>
#include <unordered_map>

class Hoge
{
public:
	Hoge()
	{
		ptr = new int(10);
		std::cout << "Hoge Constructor" << std::endl;
	}
	~Hoge()
	{
		delete ptr;
        std::cout << "Hoge Destructor" << std::endl;
    }
	
	void SayHoge() const
	{
		*ptr = 20;
		std::cout << "Hoge" << *ptr << std::endl;
	}



private:
	int* ptr;
};

//一時変数を値返しする関数
Hoge getHoge()
{
	return Hoge();
}

//一時変数の参照返しする関数　駄目
Hoge& getHoge2()
{
	return Hoge();
}

int main()
{
	//値返しの返り値をconstではない参照で受けると駄目らしいがデストラクタは呼ばれなかった
	Hoge& hoge1 = getHoge();
	hoge1.SayHoge();
	std::cout << "---------" << std::endl;
	//値返しの返り値をconst 参照で受けると寿命が延びるらしい
	const Hoge& hoge2 = getHoge();
	hoge2.SayHoge();
	std::cout << "---------" << std::endl;

	//参照先のデストラクタは既に呼ばれているが動いてしまう
	//メモリ破壊の危機
	Hoge& hoge3 = getHoge2();
	hoge3.SayHoge();
	std::cout << "---------" << std::endl;
	const Hoge& hoge4 = getHoge2();
	hoge4.SayHoge();
	std::cout << "---------" << std::endl;

	return 0;
}
