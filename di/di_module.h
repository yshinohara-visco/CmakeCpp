#pragma once
#include "../boost/di.h"
#include "logger.h"
#include <string>
#include <chrono>
#include <sstream>
#include <memory>

namespace di {

// DIモジュールの設定
inline auto createModule()
{
    namespace bdi = boost::di;

    return bdi::make_injector(
        // Loggerをシングルトンとして提供（全クラスで同じインスタンスを共有）
        bdi::bind<Logger>.in(bdi::singleton)
    );
}

} // namespace di
