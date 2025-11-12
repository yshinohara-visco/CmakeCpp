#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>

#include "divide.h"

// 最適化で削除されないようにするためのダミー変数
volatile double g_result = 0.0;
volatile bool g_success = false;

// ベンチマーク用のパラメータ
constexpr int WARMUP_ITERATIONS = 100;
constexpr int BENCHMARK_ITERATIONS = 10000;  // まずは1万回から
constexpr int TRIAL_COUNT = 3;

// 時間測定のヘルパー関数
template<typename Func>
double measure_time(Func func, int iterations)
{
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i)
    {
        func();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

// 各手法のベンチマーク関数（エラーケース: 0除算）
void benchmark_divide1_error()
{
    try
    {
        g_result = divide1(10, 0);
    }
    catch (const std::exception&)
    {
        g_success = false;
    }
}

void benchmark_divide2_error()
{
    double result;
    g_success = divide2(10, 0, result);
    if (g_success)
    {
        g_result = result;
    }
}

void benchmark_divide3_error()
{
    auto result = divide3(10, 0);
    if (result)
    {
        g_result = *result;
        g_success = true;
    }
    else
    {
        g_success = false;
    }
}

void benchmark_divide4_error()
{
    auto result = divide4(10, 0);
    if (std::holds_alternative<double>(result))
    {
        g_result = std::get<double>(result);
        g_success = true;
    }
    else
    {
        g_success = false;
    }
}

void benchmark_divide5_error()
{
    auto result = divide5(10, 0);
    if (result)
    {
        g_result = *result;
        g_success = true;
    }
    else
    {
        g_success = false;
    }
}

// ベンチマークを実行して統計を取得
template<typename Func>
void run_benchmark(const std::string& name, Func func)
{
    std::cout << "\n" << name << ":" << std::endl;

    // ウォームアップ
    measure_time(func, WARMUP_ITERATIONS);

    // 複数回測定
    std::vector<double> times;
    for (int i = 0; i < TRIAL_COUNT; ++i)
    {
        double time = measure_time(func, BENCHMARK_ITERATIONS);
        times.push_back(time);
        std::cout << "  Trial " << (i + 1) << ": " << time << " ms" << std::endl;
    }

    // 平均と標準偏差を計算
    double avg = std::accumulate(times.begin(), times.end(), 0.0) / times.size();

    double variance = 0.0;
    for (double t : times)
    {
        variance += (t - avg) * (t - avg);
    }
    double stddev = std::sqrt(variance / times.size());

    // 1回あたりの平均時間（ナノ秒）
    double avg_per_call = (avg * 1000000.0) / BENCHMARK_ITERATIONS;

    std::cout << "  Average: " << avg << " ms (± " << stddev << " ms)" << std::endl;
    std::cout << "  Per call: " << avg_per_call << " ns" << std::endl;
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Error Handling Benchmark (Division by Zero)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Iterations: " << BENCHMARK_ITERATIONS << std::endl;
    std::cout << "Trials: " << TRIAL_COUNT << std::endl;

    run_benchmark("1. Exception (std::exception)", benchmark_divide1_error);
    run_benchmark("2. Bool + Reference", benchmark_divide2_error);
    run_benchmark("3. std::optional", benchmark_divide3_error);
    run_benchmark("4. std::variant", benchmark_divide4_error);
    run_benchmark("5. std::expected", benchmark_divide5_error);

    std::cout << "\n========================================" << std::endl;
    std::cout << "Benchmark completed" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
