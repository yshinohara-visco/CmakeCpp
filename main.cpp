#include <iostream>
#include <fstream>
#include "convert.h"

int main()
{
	std::cout << "Hello World!" << std::endl;

    // このファイルはUTF-8なので全てUTF-8と思ったが、u8無しはshift-jisだった
    std::string strJa = "テスト用文字列";
    std::u8string u8strJa = u8"テスト用文字列";
    std::string strZh = "帮助文件夾"; // shift_jisでは帮が?と表示される
    std::u8string u8strZh = u8"帮助文件夾";

    // str⇔u8strでキャストすると、デバッグ実行のインテリジェンスでは無効な値が含まれると表示される
    // 恐らくstringはshift_jis、u8stringはutf-8で読もうとして失敗している
    auto str_from_u8 = convert::to_string(u8strJa);
    auto u8_from_str = convert::to_u8string(strJa);
    auto str_from_u8_zh = convert::to_string(u8strZh);
    auto u8_from_str_zh = convert::to_u8string(strZh);

    // stringはshit_jisから、u8stringはutf-8からUTF-16に変換される
    auto pathJa = fs::path(strJa);
    auto u8pathJa = fs::path(u8strJa);
    auto pathZh = fs::path(strZh);
    auto u8pathZh = fs::path(u8strZh);

    // 読もうとする型と中身が一致しないため実行時エラーになる
    // auto pathJa2 = fs::path(str_from_u8);
    // auto u8pathJa2 = fs::path(u8_from_str);
    // auto pathZh2 = fs::path(str_from_u8_zh);
    // auto u8pathZh2 = fs::path(u8_from_str_zh);

    std::ofstream ofs("test.txt");
    ofs << "strJa: " << strJa << std::endl;
    ofs << "u8strJa: " << str_from_u8 << std::endl;
    ofs << "strZh: " << strZh << std::endl;
    ofs << "u8strZh: " << str_from_u8_zh << std::endl;
    /*
    * 出力したファイルではu8無しで作成した文字列はshft_jis扱いだった
    strJa: �e�X�g�p������
    u8strJa: テスト用文字列
    strZh: ?��������
    u8strZh: 帮助文件夾
    */

    {
        auto str = pathJa.string();
        auto u8str = u8pathJa.u8string();
        auto wstr = pathJa.wstring();
    }
    {
        auto str = u8pathJa = u8pathJa.string();
        auto u8str = u8pathJa.u8string();
        auto wstr = u8pathJa.wstring();
    }
    {
        auto str = pathZh.string(); // 入れたshift_jisがそのまま出てくるらしい
        auto u8str = u8pathZh.u8string(); // 帮が表示されていた
        auto wstr = pathZh.wstring();
    }
    {
        // auto str = u8pathZh.string(); // 実行時エラーになる
        auto u8str = u8pathZh.u8string();
        auto wstr = u8pathZh.wstring();
    }
    ofs.close();



	return 0;
}
