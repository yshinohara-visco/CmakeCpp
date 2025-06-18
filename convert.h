#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;
namespace convert {

    std::u8string to_u8string(const std::string& str);

    std::string to_string(const std::u8string& u8str);



}