# C++23

C++23の機能を試す

import std を試そうとした

cmakeのCXX_STANDARDを23にするだけでいいのか？
→駄目

インストーラで「最新の v143 ビルドツール用~」を追加した。
→まだ駄目

C++20のライブラリのincludeはできた

モジュールをexportするファイルの拡張子はixxらしい。

どうもCmakeプロジェクト+VisualStudioでは標準ライブラリモジュールのビルドを有効にできないらしい。
自前でコピーしてきてビルドに含めたらできるとか？
→コピーして来るか、直接ビルド対象に含めてもビルドできた。
しかしフルパスをcmakeに含むのは抵抗がある