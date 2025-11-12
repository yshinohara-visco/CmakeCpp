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