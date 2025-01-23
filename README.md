# CmakeCpp

## 概要

Cmake プロジェクトで作成した DLL を完全に別のプロジェクトから使用するには何が要るかを確認した。

## 詳細

LibProject で静的ライブラリ ModuleStatic と動的ライブラリ ModuleShared を作成している。
LibPriject 内からこれらを使用するには add_subdirectory からの target_link_directories でいい。

全く別の Cmake プロジェクト ExeProject から使用するには最低限何が必要か？
cmake で適切にインストールしたり出力する方法が良く分からなかったので、手作業でコピーしながら試した。

~~まず、コンパイルに ModuleStaic.h と ModuleShared.h の両方が必要だった。~~
~~これは ModuleShared.h が ModuleStaic.h をインクルードするから。~~

~~リンクには ModuleStatic.lib と ModuleShared.lib の両方が必要だった。ModuleShared.lib に Static の内容が含まれるということは無いらしい。~~

実行には ModuleShared.dll が必要だった。

~~一部を DLL にして公開したかったが、結局開発には全てのヘッダ、lib が必要と考えた方が良さそう。~~

色々勘違いしていたので再検証した
Static.lib が必要になっていたのはそのように書いていたからだった。
1．ModuleShared.h が ModuleStatic.h をインクルードしなければ、ExeProject には ModuleShared.h だけあればいい
2．この上で CmakeLists.txt に ModuleStatic を書かなければ、ビルドに ModuleStatic.lib は必要ない
3．実行には ModuleShared.dll が必要

ヘッダから Static の要素を隠せば、Shared だけが参照する Static ライブラリは利用者から見える必要は無い。
