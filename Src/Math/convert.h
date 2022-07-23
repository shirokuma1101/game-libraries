#pragma once

#include "constant.h"

namespace convert {

    using constant::fPI;
    using constant::dPI;

    template<class T> constexpr T    ToSquare(T base)    noexcept { return base * base; }
    template<class T> constexpr void ToSquare(T* base)            { *base *= *base; }

    template<class T> constexpr T    ToHalf(T num)       noexcept { return num * 0.5f; }
    template<>        constexpr int  ToHalf(int num)     noexcept { return num * 0.5; }
    template<>        constexpr long ToHalf(long num)    noexcept { return num * 0.5; }
    template<class T> constexpr void ToHalf(T* num)               { *num *= 0.5f; }
    template<>        constexpr void ToHalf(int* num)             { *num *= 0.5; }
    template<>        constexpr void ToHalf(long* num)            { *num *= 0.5; }

    template<class T> constexpr T    ToQuarter(T num)    noexcept { return num * 0.25f; }
    template<>        constexpr int  ToQuarter(int num)  noexcept { return num * 0.25; }
    template<>        constexpr long ToQuarter(long num) noexcept { return num * 0.25; }
    template<class T> constexpr void ToQuarter(T* num)            { *num *= 0.25f; }
    template<>        constexpr void ToQuarter(int* num)          { *num *= 0.25; }
    template<>        constexpr void ToQuarter(long* num)         { *num *= 0.25; }


    template<class T> constexpr T    ToRadians(T deg)             { return deg * (fPI / 180.f); }
    template<>        constexpr int  ToRadians(int deg)  noexcept { return deg * (dPI / 180); }
    template<>        constexpr long ToRadians(long deg) noexcept { return deg * (dPI / 180); }
    template<class T> constexpr void ToRadians(T* deg)            { *deg *= (fPI / 180.f); }
    template<>        constexpr void ToRadians(int* deg)          { *deg *= (dPI / 180); }
    template<>        constexpr void ToRadians(double* deg)       { *deg *= (dPI / 180); }

    template<class T> constexpr T    ToDegrees(T rad)             { return rad * (180.f / fPI); }
    template<>        constexpr int  ToDegrees(int rad)  noexcept { return rad * (180 / dPI); }
    template<>        constexpr long ToDegrees(long rad) noexcept { return rad * (180 / dPI); }
    template<class T> constexpr void ToDegrees(T* rad)            { *rad *= (180.f / fPI); }
    template<>        constexpr void ToDegrees(int* rad)          { *rad *= (180 / dPI); }
    template<>        constexpr void ToDegrees(double* rad)       { *rad *= (180 / dPI); }

}
