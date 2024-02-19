import std; //現状std.ixxをビルド対象に含めると使える
import myModule;

int main()
{
	hello();
	
	std::println("use std module!");

	return 0;
}
