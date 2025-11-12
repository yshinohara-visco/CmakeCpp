#pragma once

#include <optional>
#include <variant>
#include <string>
#include <expected>


// 「失敗するかもしれない関数」の簡単な例として割り算を考える
// doubleならinfとかnanとかで表現されるが、ここではあえて例外を使う

// エラーコード
enum class DivideError
{
	DivisionByZero,
	InvalidInput,
	// 将来的に追加する可能性のあるエラー
};

// エラーコードを文字列に変換
inline const char* to_string(DivideError error)
{
	switch (error)
	{
	case DivideError::DivisionByZero:
		return "Division by zero";
	case DivideError::InvalidInput:
		return "Invalid input";
	default:
		return "Unknown error";
	}
}

// 1.結果を返り値で返し、エラーは例外で伝えるバージョン
double divide1(int a, int b);

// 2.結果を参照引数で返し、成否は戻り値で伝えるバージョン
bool divide2(int a, int b, double& result);

// 3.結果をstd::optionalで返し、成否を区別するバージョン
std::optional<double> divide3(int a, int b);

// 4.結果をstd::variantで返し、成功とエラーを区別するバージョン
std::variant<double, DivideError> divide4(int a, int b);

// 5.結果をstd::expectedで返し、成功とエラーを区別するバージョン
std::expected<double, DivideError> divide5(int a, int b);
