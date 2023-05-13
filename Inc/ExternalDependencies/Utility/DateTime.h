/**
 * @file DateTime.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_UTILITY_DATETIME_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_UTILITY_DATETIME_H_

#include <Windows.h>

/**
 * @namespace date_time
 * @brief date time utility
 */
namespace date_time {

    union RealTime {
        ULARGE_INTEGER ui;
        FILETIME       ft;
    };

    inline UINT64 GetRealTime() {
        SYSTEMTIME st{};
        SecureZeroMemory(&st, sizeof(st));
        GetSystemTime(&st);

        RealTime rt{};
        SecureZeroMemory(&rt, sizeof(rt));
        SystemTimeToFileTime(&st, &rt.ft);

        return rt.ui.QuadPart;
    }

    inline SYSTEMTIME GetSystemTime(UINT64 real_time) {
        RealTime rt{};
        SecureZeroMemory(&rt, sizeof(rt));
        rt.ui.QuadPart = real_time;

        SYSTEMTIME st{};
        SecureZeroMemory(&st, sizeof(st));
        FileTimeToSystemTime(&rt.ft, &st);

        return st;
    }

}

#endif
