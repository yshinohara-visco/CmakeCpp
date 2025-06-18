#include "convert.h"

namespace fs = std::filesystem;
namespace convert {

    std::u8string to_u8string(const std::string& str) {
        return std::u8string(str.begin(), str.end());
    }

    std::string to_string(const std::u8string& u8str) {
        return std::string(u8str.begin(), u8str.end());
    }


}