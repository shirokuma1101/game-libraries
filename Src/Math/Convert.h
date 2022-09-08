#pragma once

#ifndef GAME_LIBRARIES_MATH_CONVERT_H_
#define GAME_LIBRARIES_MATH_CONVERT_H_

#include <list>
#include <vector>

#include "Math/Constant.h"
#include "Utility/Macro.h" 
                                                                                                                  
#define CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(func_name, arg_name, float_expr, double_expr)                         \
NAMESPACE_EXTERNAL_BEGIN                                                                                          \
NAMESPACE_INTERNAL_BEGIN                                                                                          \
template<class ArgTy, class>                                                                                      \
struct func_name                { static constexpr auto Func(ArgTy arg_name) noexcept { return float_expr; } };   \
template<class ArgTy>                                                                                             \
struct func_name<ArgTy, double> { static constexpr auto Func(ArgTy arg_name) noexcept { return double_expr; } };  \
NAMESPACE_INTERNAL_END                                                                                            \
template<class ArgTy>                                                                                             \
constexpr auto func_name(ArgTy arg_name) noexcept { return detail::func_name<ArgTy, ArgTy>::Func(arg_name); }     \
template<class ExpectTy, class ArgTy>                                                                             \
constexpr auto func_name(ArgTy arg_name) noexcept { return detail::func_name<ArgTy, ExpectTy>::Func(arg_name); }  \
NAMESPACE_EXTERNAL_END

namespace convert {

    template<class CastTy, class T>
    constexpr CastTy SizeOf(const T& arg) {
        return static_cast<CastTy>(sizeof(arg));
    }

    template<class T> constexpr T      ToSquare(const T& base) noexcept { return base * base; }
    template<class T> constexpr void   ToSquare(T* base)                { *base *= *base; }
    /*
    NAMESPACE_EXTERNAL_BEGIN
    NAMESPACE_INTERNAL_BEGIN
    template<class ArgTy, class> struct ToHalf                   { static constexpr auto Func(ArgTy num) noexcept { return num * 0.5f; } };
    template<class ArgTy>        struct ToHalf<ArgTy, double>    { static constexpr auto Func(ArgTy num) noexcept { return num * 0.5; } };
    template<class ArgTy, class> struct ToQuarter                { static constexpr auto Func(ArgTy num) noexcept { return num * 0.25f; } };
    template<class ArgTy>        struct ToQuarter<ArgTy, double> { static constexpr auto Func(ArgTy num) noexcept { return num * 0.25; } };
    template<class ArgTy, class> struct ToRadians                { static constexpr auto Func(ArgTy deg) noexcept { return deg * (constant::fPI / 180.f); } };
    template<class ArgTy>        struct ToRadians<ArgTy, double> { static constexpr auto Func(ArgTy deg) noexcept { return deg * (constant::dPI / 180.0); } };
    template<class ArgTy, class> struct ToDegrees                { static constexpr auto Func(ArgTy rad) noexcept { return rad * (180.f / constant::fPI); } };
    template<class ArgTy>        struct ToDegrees<ArgTy, double> { static constexpr auto Func(ArgTy rad) noexcept { return rad * (180.0 / constant::dPI); } };
    NAMESPACE_INTERNAL_END
    template<class ArgTy>                 constexpr auto ToHalf(ArgTy num)    noexcept { return detail::ToHalf<ArgTy, ArgTy>::Func(num); }
    template<class ExpectTy, class ArgTy> constexpr auto ToHalf(ArgTy num)    noexcept { return detail::ToHalf<ArgTy, ExpectTy>::Func(num); }
    template<class ArgTy>                 constexpr auto ToQuarter(ArgTy num) noexcept { return detail::ToQuarter<ArgTy, ArgTy>::Func(num); }
    template<class ExpectTy, class ArgTy> constexpr auto ToQuarter(ArgTy num) noexcept { return detail::ToQuarter<ArgTy, ExpectTy>::Func(num); }
    template<class ArgTy>                 constexpr auto ToRadians(ArgTy deg) noexcept { return detail::ToRadians<ArgTy, ArgTy>::Func(deg); }
    template<class ExpectTy, class ArgTy> constexpr auto ToRadians(ArgTy deg) noexcept { return detail::ToRadians<ArgTy, ExpectTy>::Func(deg); }
    template<class ArgTy>                 constexpr auto ToDegrees(ArgTy rad) noexcept { return detail::ToDegrees<ArgTy, ArgTy>::Func(rad); }
    template<class ExpectTy, class ArgTy> constexpr auto ToDegrees(ArgTy rad) noexcept { return detail::ToDegrees<ArgTy, ExpectTy>::Func(rad); }
    NAMESPACE_EXTERNAL_END
    */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToHalf,    num, num * 0.5f,                    num * 0.5                    );
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToQuarter, num, num * 0.25f,                   num * 0.25                   );
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToRadians, deg, deg * (constant::fPI / 180.f), deg * (constant::dPI / 180.0));
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToDegrees, rad, rad * (180.f / constant::fPI), rad * (180.0 / constant::dPI));

    
    // 1S = 1000MS = 1000000US = 1000000000NS

    template<class T> constexpr auto    SToNS(T t)  noexcept { return t * 1000000000; }
    template<class T> constexpr auto    SToUS(T t)  noexcept { return t * 1000000;    }
    template<class T> constexpr auto    SToMS(T t)  noexcept { return t * 1000;       }
    
    template<class T> constexpr auto    MSToNS(T t) noexcept { return t * 1000000;  }
    template<class T> constexpr auto    MSToUS(T t) noexcept { return t * 1000;     }
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(MSToS,  t, t * 0.001f,       t * 0.001      );
    
    template<class T> constexpr auto    USToNS(T t) noexcept { return t * 1000;     }
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(USToMS, t, t * 0.001f,       t * 0.001      );
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(USToS,  t, t * 0.000001f,    t * 0.000001   );
    
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToUS, t, t * 0.001f,       t * 0.001      );
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToMS, t, t * 0.000001f,    t * 0.000001   );
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToS,  t, t * 0.000000001f, t * 0.000000001);

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

#endif
