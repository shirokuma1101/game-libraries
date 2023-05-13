/**
 * @file String.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_UTILITY_STRING_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_UTILITY_STRING_H_

#include <string>
#include <string_view>

#include "strconv.h"

/**
 * @namespace string
 * @brief string utility
 */
namespace string {

    inline std::u16string StrToUtf16(std::string_view str) {
        //auto wstr = utf8_to_wide(str.data());
        auto wstr = sjis_to_wide(str.data());
        return std::u16string(wstr.begin(), wstr.end());
    }

}

#endif
