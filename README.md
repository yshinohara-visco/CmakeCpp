# CmakeCpp

## 概要
cmakeを用いたC++のプロジェクト。

## 用途
「失敗するかもしれない処理」で、成否を区別する方法は色々ある。
思いついたり調べて出てくる方法を列挙して比較してみる。
標準ライブラリは例外が返るか例外を受け取る引数を渡すケースが多いが、忘れがちなので好きじゃない。

幾つか試してみたが、機能や使い方的にexpected<T, E>が一番良さそうに思えた。
が、書くのが一回り面倒にはなるので最初っからガチガチにexpected<T, E>で書くのもどうかと思う。
一旦はシンプルな方法で実装して、必要に応じてexpected<T, E>に置き換えるのが良いかもしれない。
でもそうすると絶対置き換え漏れが出てくるのも考えどころ。

## 実装した5つの手法

1. **例外（std::exception）** - 最もシンプルだがtry-catch漏れのリスク
2. **bool戻り値 + 参照引数** - 古典的だが詳細なエラー情報が得られない
3. **std::optional** - 成否のみを区別、C++17以降
4. **std::variant** - 成功/失敗の型を区別できるが使い分けが煩雑
5. **std::expected** - 最も自然な書き方、C++23以降

## ビルド要件

- C++23対応コンパイラ（std::expectedを使用）
- CMake

## 実行例

```
divide1: 3.33333
Error in divide1: Division by zero
divide2: 3.33333
Failed divide2
divide3: 3.33333
Failed divide3
divide4: 3.33333
Error in divide4: Division by zero
divide5: 3.33333
Error in divide5: Division by zero
```

## 補足

- 例外のパフォーマンスコストは正常系では無視できるレベルだが、頻繁に例外が発生する場合は注意
- std::expectedはRustのResult<T, E>に影響を受けた設計
- 既存コードベースとの整合性も考慮して手法を選択すべき

## 実行時間の比較1
```
========================================
Error Handling Benchmark (Division by Zero)
========================================
Iterations: 10000
Trials: 3

1. Exception (std::exception):
  Trial 1: 1802.32 ms
  Trial 2: 1763.49 ms
  Trial 3: 1794.83 ms
  Average: 1786.88 ms (ﾂｱ 16.8215 ms)
  Per call: 178688 ns

2. Bool + Reference:
  Trial 1: 0.0196 ms
  Trial 2: 0.0212 ms
  Trial 3: 0.0197 ms
  Average: 0.0201667 ms (ﾂｱ 0.000731817 ms)
  Per call: 2.01667 ns

3. std::optional:
  Trial 1: 0.0218 ms
  Trial 2: 0.0219 ms
  Trial 3: 0.0218 ms
  Average: 0.0218333 ms (ﾂｱ 4.71405e-05 ms)
  Per call: 2.18333 ns

4. std::variant:
  Trial 1: 2.4265 ms
  Trial 2: 2.5105 ms
  Trial 3: 2.4448 ms
  Average: 2.4606 ms (ﾂｱ 0.0360669 ms)
  Per call: 246.06 ns

5. std::expected:
  Trial 1: 0.5776 ms
  Trial 2: 0.3921 ms
  Trial 3: 0.3968 ms
  Average: 0.4555 ms (ﾂｱ 0.0863591 ms)
  Per call: 45.55 ns

========================================
Benchmark completed
========================================
```

exceptionが圧倒的に遅い。variantとexpectedもそこそこ遅いが、これはstringを使用していたのが原因と思われる。

## 実行時間の比較2（文字列を使わない場合）
```
========================================
Error Handling Benchmark (Division by Zero)
========================================
Iterations: 10000
Trials: 3

1. Exception (std::exception):
  Trial 1: 1800.45 ms
  Trial 2: 1782.72 ms
  Trial 3: 1814.89 ms
  Average: 1799.35 ms (ﾂｱ 13.1548 ms)
  Per call: 179935 ns

2. Bool + Reference:
  Trial 1: 0.0218 ms
  Trial 2: 0.0219 ms
  Trial 3: 0.0218 ms
  Average: 0.0218333 ms (ﾂｱ 4.71405e-05 ms)
  Per call: 2.18333 ns

3. std::optional:
  Trial 1: 0.0217 ms
  Trial 2: 0.0218 ms
  Trial 3: 0.0218 ms
  Average: 0.0217667 ms (ﾂｱ 4.71405e-05 ms)
  Per call: 2.17667 ns

4. std::variant:
  Trial 1: 0.0225 ms
  Trial 2: 0.0226 ms
  Trial 3: 0.0225 ms
  Average: 0.0225333 ms (ﾂｱ 4.71405e-05 ms)
  Per call: 2.25333 ns

5. std::expected:
  Trial 1: 0.0218 ms
  Trial 2: 0.0217 ms
  Trial 3: 0.0218 ms
  Average: 0.0217667 ms (ﾂｱ 4.71405e-05 ms)
  Per call: 2.17667 ns

========================================
Benchmark completed
========================================
```
stringを使わないようにすると、exception以外はほぼ同じ速度になった。
危うくexpectedが遅いと誤解するところだった。