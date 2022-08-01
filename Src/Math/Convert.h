#pragma once

#include <vector>
#include <list>

#include <Math/Constant.h>

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

    template<class T>          constexpr T SToNS(T t)  { return t * 1000000000; }
    template<class T>          constexpr T SToUS(T t)  { return t * 1000000; }
    template<class T>          constexpr T SToMS(T t)  { return t * 1000; }
    template<class T>          constexpr T MSToNS(T t) { return t * 1000000; }
    template<class T>          constexpr T MSToUS(T t) { return t * 1000; }
    template<class T>          constexpr T USToNS(T t) { return t * 1000; }

    template<template<class> class Container, class T>
    inline auto ToContainer(T iterable) {
        typename T::value_type Ty;
        Container<decltype(Ty)> container;
        for (auto&& e : iterable) {
            container.insert(container.end(), e);
        }
        return container;
    }

    template<class T, class U>
    inline std::vector<T> ToVector(U iterable) {
        return ToContainer<std::vector>(iterable);
    }

    template<class T, class U>
    inline std::list<T> ToList(U iterable) {
        return ToContainer<std::list>(iterable);
    }

    template<class T, class Iter = decltype(std::begin(std::declval<T>()))>
    inline auto Enumerate(T&& iterable) {
        struct iterator {
            size_t i;
            Iter iter;
            bool operator!= (const iterator& other) const { return iter != other.iter; }
            void operator++ ()                            { ++i; ++iter; }
            auto operator*  ()                      const { return std::tuple(i, *iter); }
        };
        struct iterable_wrapper {
            T iterable;
            auto begin() { return iterator{ 0, std::begin(iterable) }; }
            auto end()   { return iterator{ 0, std::end(iterable) }; }
        };
        return iterable_wrapper{ std::forward<T>(iterable) };
    }

    inline float Normalize(float x, float x_min, float x_max, float y_start = 0.f, float y_end = 1.f) {
        
        if (x_min != 0.f) {
            x -= x_min;
            x_max -= x_min;
            x_min = 0.f;
        }

        if (y_start != 0.f) {
            y_end -= y_start;
        }

        float y = x / x_max;
        return (y_end * y) + y_start;
    }

}
