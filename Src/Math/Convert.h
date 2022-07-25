#pragma once

#include <vector>
#include <list>

#include "Constant.h"

namespace convert {

    using constant::fPI;
    using constant::dPI;

    template<class T> constexpr T    ToSquare(T base) noexcept { return base * base; }
    template<class T> constexpr void ToSquare(T* base)         { *base *= *base; }

    template<class T>          constexpr float ToHalf(T num) noexcept { return num * 0.5f; }
    template<class U, class T> constexpr U     ToHalf(T num) noexcept { return num * 0.5; }
    template<class T>          constexpr void  ToHalf(T* num)         { *num *= 0.5f; }
    //template<class T>          constexpr void  ToHalf(T* num)         { *num *= 0.5; }

    template<class T>          constexpr float ToQuarter(T num) noexcept { return num * 0.25f; }
    template<class U, class T> constexpr U     ToQuarter(T num) noexcept { return num * 0.25; }
    template<class T>          constexpr void  ToQuarter(T* num)         { *num *= 0.25f; }
    //template<class T>          constexpr void  ToQuarter(T* num)         { *num *= 0.25; }


    template<class T>          constexpr float ToRadians(T deg) noexcept { return deg * (fPI / 180.f); }
    template<class U, class T> constexpr U     ToRadians(T deg) noexcept { return deg * (fPI / 180); }
    template<class T>          constexpr void  ToRadians(T* deg)         { *deg *= (fPI / 180.f); }
    //template<class T>          constexpr void  ToRadians(T* deg)         { *deg *= (fPI / 180); }

    template<class T>          constexpr float ToDegrees(T rad) noexcept { return rad * (180.f / fPI); }
    template<class U, class T> constexpr U     ToDegrees(T rad) noexcept { return rad * (180 / fPI); }
    template<class T>          constexpr void  ToDegrees(T* rad)         { *rad *= (180.f / fPI); }
    //template<class T>          constexpr void  ToDegrees(T* rad)         { *rad *= (180 / fPI); }


    template<class T>          constexpr float NSToS(T t) { return t * 0.000000001f; }
    template<class U, class T> constexpr U     NSToS(T t) { return t * 0.000000001; }
    template<class T>          constexpr float USToS(T t) { return t * 0.000001f; }
    template<class U, class T> constexpr U     USToS(T t) { return t * 0.000001; }
    template<class T>          constexpr float MSToS(T t) { return t * 0.001f; }
    template<class U, class T> constexpr U     MSToS(T t) { return t * 0.001; }


    template<class T, class U>
    inline std::vector<T> ToVector(U iterable) {
        std::vector<T> list;
        for (auto&& e : iterable) {
            list.push_back(e);
        }
        return list;
    }

    template<class T, class U>
    inline std::list<T> ToList(U iterable) {
        std::list<T> list;
        for (auto&& e : iterable) {
            list.push_back(e);
        }
        return list;
    }

}
