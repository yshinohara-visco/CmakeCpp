/*
外部に公開するヘッダ
ここの定義に環境依存なコードを含めてはいけない
(cmakeで頑張ればOS毎のヘッダを出力はできなくはない?)
*/

#pragma once

void SayHello();
void SayMoreHello();
void SayMoreMoreHello();

void SayGoodbye();