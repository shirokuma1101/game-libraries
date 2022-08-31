#pragma once

#include <string>
#include <string_view>

#include "strconv.h"

namespace string {
    inline std::u16string StrToUtf16(std::string_view str) {
        //auto wstr = utf8_to_wide(str.data());
        auto wstr = sjis_to_wide(str.data());
        return std::u16string(wstr.begin(), wstr.end());
    }
}
