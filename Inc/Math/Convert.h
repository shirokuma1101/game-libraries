/**
 * @file Convert.h
 * @author shirokuma1101
 * @version 1.0
 * @date 2022-12-19
 *
 * @copyright Copyright (c) 2022 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_MATH_CONVERT_H_
#define GAME_LIBRARIES_MATH_CONVERT_H_

#include <list>
#include <tuple>
#include <type_traits>
#include <vector>

#include "Math/Constant.h"
#include "Utility/Macro.h"

namespace convert {

#define CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(func_name, arg_name, float_expr, double_expr) \
MACRO_NAMESPACE_EXTERNAL_BEGIN                                                            \
MACRO_NAMESPACE_INTERNAL_BEGIN                                                            \
template<class ArgTy, class>                                                              \
struct func_name {                                                                        \
    static constexpr auto Func(const ArgTy& arg_name) noexcept {                          \
        return float_expr;                                                                \
    }                                                                                     \
};                                                                                        \
template<class ArgTy>                                                                     \
struct func_name<ArgTy, double> {                                                         \
    static constexpr auto Func(const ArgTy& arg_name) noexcept {                          \
        return double_expr;                                                               \
    }                                                                                     \
};                                                                                        \
MACRO_NAMESPACE_INTERNAL_END                                                              \
template<class ArgTy>                                                                     \
constexpr auto func_name(const ArgTy& arg_name) noexcept {                                \
    return detail::func_name<ArgTy, ArgTy>::Func(arg_name);                               \
}                                                                                         \
template<class ExpectTy, class ArgTy>                                                     \
constexpr auto func_name(const ArgTy& arg_name) noexcept {                                \
    return detail::func_name<ArgTy, ExpectTy>::Func(arg_name);                            \
}                                                                                         \
MACRO_NAMESPACE_EXTERNAL_END


    /**************************************************
    * Math conversion
    **************************************************/

    /**
     * Calculates the square of a value.
     * @tparam T The type of the input value.
     * @param base The input value to square.
     * @return The square of the input value.
     */
    template<class T> constexpr T ToSquare(const T& base) noexcept {
        return base * base;
    }

    /**
     * @brief Calculates half of the value of the input parameter.
     * @tparam T The type of the input parameter.
     * @param num A reference to the input parameter.
     * @return The result of dividing the input value by two.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToHalf, num,
        num * 0.5f, num * 0.5
    );
    /**
     * @brief Calculates a quarter of the value of the input parameter.
     * @tparam T The type of the input parameter.
     * @param num A reference to the input parameter.
     * @return The result of dividing the input value by four.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToQuarter, num,
        num * 0.25f, num * 0.25
    );

    /**
     * @brief Calculates the percentage of the input value.
     * @tparam T The type of the input parameter.
     * @param num A reference to the input parameter.
     * @return The result of multiplying the input value by 100.
     */
    template<class T> constexpr T ToPercent(const T& num) noexcept {
        return num * 100;
    }
    /**
     * @brief Calculates the original value from the percentage.
     * @tparam T The type of the input parameter.
     * @param num A reference to the input parameter.
     * @return The result of dividing the input value by 100.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToUndoPercent, num,
        num * 0.01f, num * 0.01
    );

    /**
     * @brief Converts degrees to radians.
     * @tparam T The type of the input parameter.
     * @param deg A reference to the input parameter in degrees.
     * @return The input value converted to radians.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToRadians, deg,
        deg * (constant::fPI / 180.f), deg * (constant::dPI / 180.0)
    );
    /**
     * @brief Converts radians to degrees.
     * @tparam T The type of the input parameter.
     * @param rad A reference to the input parameter in radians.
     * @return The input value converted to degrees.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToDegrees, rad,
        rad * (180.f / constant::fPI), rad * (180.0 / constant::dPI)
    );


    /* Time conversion */
    // 1S = 1000MS = 1000000US = 1000000000NS

    // S to

    /**
     * @brief Converts a value in seconds to nanoseconds.
     * @tparam T The type of the input value.
     * @param t The input value in seconds.
     * @return The input value in nanoseconds.
     */
    template<class T> constexpr auto SToNS(T t) noexcept { return t * 1000000000; }
    /**
     * @brief Converts a value in seconds to microseconds.
     * @tparam T The type of the input value.
     * @param t The input value in seconds.
     * @return The input value in microseconds.
     */
    template<class T> constexpr auto SToUS(T t) noexcept { return t * 1000000; }
    /**
     * @brief Converts a value in seconds to milliseconds.
     * @tparam T The type of the input value.
     * @param t The input value in seconds.
     * @return The input value in milliseconds.
     */
    template<class T> constexpr auto SToMS(T t) noexcept { return t * 1000; }

    // MS to

    /**
     * Converts a value in milliseconds to nanoseconds.
     * @tparam T The type of the input value.
     * @param t The input value in milliseconds.
     * @return The input value in nanoseconds.
     */
    template<class T> constexpr auto MSToNS(T t) noexcept { return t * 1000000; }
    /**
     * Converts a value in milliseconds to microseconds.
     * @tparam T The type of the input value.
     * @param t The input value in milliseconds.
     * @return The input value in microseconds.
     */
    template<class T> constexpr auto MSToUS(T t) noexcept { return t * 1000; }
    /**
     * Converts a value in milliseconds to seconds.
     * @tparam T The type of the input value.
     * @param t The input value in milliseconds.
     * @return The input value in seconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(MSToS, t, t * 0.001f, t * 0.001);

    // US to

    /**
     * Converts a value in microseconds to nanoseconds.
     * @tparam T The type of the input value.
     * @param t The input value in microseconds.
     * @return The input value in nanoseconds.
     */
    template<class T> constexpr auto USToNS(T t) noexcept { return t * 1000; }
    /**
     * Converts a value in microseconds to milliseconds.
     * @tparam T The type of the input value.
     * @param t The input value in microseconds.
     * @return The input value in milliseconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(USToMS, t, t * 0.001f, t * 0.001);
    /**
     * Converts a value in microseconds to seconds.
     * @tparam T The type of the input value.
     * @param t The input value in microseconds.
     * @return The input value in seconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(USToS, t, t * 0.000001f, t * 0.000001);

    // NS to

    /**
     * Converts a value in nanoseconds to microseconds.
     * @tparam T The type of the input value.
     * @param t The input value in nanoseconds.
     * @return The input value in microseconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToUS, t, t * 0.001f, t * 0.001);
    /**
     * Converts a value in nanoseconds to milliseconds.
     * @tparam T The type of the input value.
     * @param t The input value in nanoseconds.
     * @return The input value in milliseconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToMS, t, t * 0.000001f, t * 0.000001);
    /**
     * Converts a value in nanoseconds to seconds.
     * @tparam T The type of the input value.
     * @param t The input value in nanoseconds.
     * @return The input value in seconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToS, t, t * 0.000000001f, t * 0.000000001);


    /**************************************************
    * Container conversion
    **************************************************/

    /**
     * Converts an iterable object to a container.
     * @tparam Container The type of the container to create.
     * @tparam T The type of the iterable object.
     * @param iterable The iterable object to convert.
     * @return A container with the elements from the iterable object.
     */
    template<template<class> class Container, class T>
    inline auto ToContainer(const T& iterable) {
        Container<typename T::value_type> container;
        for (auto&& e : iterable) {
            container.insert(container.end(), e);
        }
        return container;
    }
    /**
     * @brief Convert the elements in an iterable and return them in a new container.
     * @tparam Container The type of container to return.
     * @tparam ConvertFunc The type of the conversion function.
     * @tparam T The type of the iterable.
     * @param iterable The iterable to convert the elements of.
     * @param convert_func The function to use to convert the elements.
     * @return A new container with the converted elements.
     */
    template<template<class> class Container, class ConvertFunc, class T>
    inline auto ToConvertedContainer(const T& iterable, ConvertFunc&& convert_func) {
        Container<decltype(convert_func(*iterable.begin()))> container;
        for (auto&& e : iterable) {
            container.insert(container.end(), convert_func(e));
        }
        return container;
    }


    /**************************************************
    * Enumerate
    **************************************************/

    template<class T, class Iter = decltype(std::begin(std::declval<T>()))>
    inline auto Enumerate(T&& iterable) {
        struct Iterator {
            size_t i;
            Iter iter;
            bool operator!=(const Iterator& other) const { return iter != other.iter; }
            void operator++()                            { ++i; ++iter; }
            auto operator*()                       const { return std::tuple(i, *iter); }
        };
        struct IterableWrapper {
            T iterable;
            auto begin() { return Iterator{ 0, std::begin(iterable) }; }
            auto end()   { return Iterator{ 0, std::end(iterable) }; }
        };
        return IterableWrapper{ std::forward<T>(iterable) };
    }


    /**************************************************
    * Other
    **************************************************/

    template<class CastTy, class T>
    constexpr CastTy SizeOf(const T& arg) {
        return static_cast<CastTy>(sizeof(arg));
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
