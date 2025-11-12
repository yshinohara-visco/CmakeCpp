#include <iostream>

#include "divide.h"

const int a = 10;
const int b = 3;
const int c = 0;

/*
例外でエラーを伝える場合
exceptionからエラーの内容が取得出来る
一番シンプルだが、try-catchを忘れると落ちるのが難点
例外が発生した時のcatchのコストも気になる
*/
void run1(int num1, int num2)
{
    try
    {
        std::cout << "divide1: " << divide1(num1, num2) << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Error in divide1: " << e.what() << std::endl;
    }
}

/*
戻り値で成否を伝える場合
boolの場合、失敗したこと以上は分からない
返り値をboolからenumとかに変えればもう少し情報を持たせられる
実行する前にresultを用意する流れが好きじゃない、が事前にメモリを確保する点は有効な場面が有りそう
resultとして渡すのがstringやvectorだった場合、既存の内容は消されるのか?追記なのか?とかで迷う時がある
*/
void run2(int num1, int num2)
{
    double result;
    if (divide2(num1, num2, result))
    {
        std::cout << "divide2: " << result << std::endl;
    }
    else
    {
        std::cout << "Failed divide2" << std::endl;
    }
}

/*
std::optionalで成否を区別する場合
失敗したこと以上は分からない
divide2のように情報を増やすことはできない
書き方や解釈が自然に書ける点が好き
成否以上の興味が無いことがハッキリしている場合に有効
*/
void run3(int num1, int num2)
{
    auto result = divide3(num1, num2);
    if (result)
    {
        std::cout << "divide3: " << *result << std::endl;
    }
    else
    {
        std::cout << "Failed divide3" << std::endl;
    }
}

/*
std::variantで成否を区別する場合
成功とエラーの内容を区別できる
stringをenumとかに変えれば区別しやすくなる
variantの中身を確認するのが面倒
エラーだけでなく、成功の中でも種類がある時しか使いたくない
*/
void run4(int num1, int num2)
{
    auto result = divide4(num1, num2);
    if (std::holds_alternative<double>(result))
    {
        std::cout << "divide4: " << std::get<double>(result) << std::endl;
    }
    else
    {
        std::cout << "Error in divide4: " << std::get<std::string>(result) << std::endl;
    }
}

/*
std::expectedで成否を区別する場合
成功とエラーの内容を区別できる
C++23の最新の方法
「結果又はエラーを受け取る」という目的とコードが一番合っていそう
*/
void run5(int num1, int num2)
{
    auto result = divide5(num1, num2);
    if (result)
    {
        std::cout << "divide5: " << *result << std::endl;
    }
    else
    {
        std::cout << "Error in divide5: " << result.error() << std::endl;
    }
}

/*
総評
失敗する可能性も考慮して関数を設計する場合、expectedが一番自然そう
C++23以降という点がネックになりそうではある
例外はtry-catchを忘れると落ちるので怖い
処理コストがカツカツで出来る限り削りたい場合は他の方法の出番もあるかも
1,2: 使いどころは有りそう
3,4: 使う意味は薄いかも
5: 一番綺麗だがC++23以降が必要
*/

int main()
{
    run1(a, b);
    run1(a, c);
    run2(a, b);
    run2(a, c);
    run3(a, b);
    run3(a, c);
    run4(a, b);
    run4(a, c);
    run5(a, b);
    run5(a, c);

	return 0;
}