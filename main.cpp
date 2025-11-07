#include <iostream>
#include <chrono>
#include <thread>

#include "my_class.h"

int main()
{
	std::cout << "Hello World!" << std::endl;

	{
        Holder h;
        h.Launch1();
	} // h解放
    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        Holder h;
        h.Launch2();
    } // h解放
    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        Holder h;
        h.Launch2b();
    } // h解放
    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        Holder h;
        h.Launch3();
    } // h解放
    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        Holder h;
        h.Launch4();
    } // h解放
    std::this_thread::sleep_for(std::chrono::seconds(2));

    /*
    実行結果
    Hello World!
    num_: 11
    num_: 22
    num_: 222
    num_: 33
    spb_ has been released.
    
    shared_ptrとweak_ptrを使用した処理以外は解放済みの要素にアクセスした。
    落ちなかったのは偶然メモリ上にそのまま残っていたからであり、不定動作である。
    */

    /*
    pb_, upb_をデストラクタで明示的に解放した場合の実行結果
    Hello World!
    num_: 11
    num_: 22
    pb_ is nullptr.
    upb_ has been released.
    spb_ has been released.

    デストラクタで解放した場合、Launch2, Launch3内でnullptrチェックが働いた。
    upb_については暗黙の解放でも同様の挙動になると思ったが、ならなかった。
    そもそもhと一緒に各メンバ変数が解放されているので、メンバ変数を参照している1,2b,3は論外だった。

    C++の作法的には「解放されているかもしれないインスタンスへの参照」はshared_ptr/weak_ptrで扱うべき、ということか。
    よく考えると、生ポインタとunique_ptrはヌルチェックが通ったとしても、その後の使用中に解放される可能性がある。
    そうなると、shared_ptr/weak_ptrを使い、参照中はlockして解放されないように保護するしかない。
    */

	return 0;
}
