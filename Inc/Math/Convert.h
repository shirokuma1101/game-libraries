/**
 * @file Convert.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
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

/**
 * @namespace convert
 * @brief Namespace for conversion functions.
 */
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
        num * 0.5f,
        num * 0.5);
    /**
     * @brief Calculates a quarter of the value of the input parameter.
     * @tparam T The type of the input parameter.
     * @param num A reference to the input parameter.
     * @return The result of dividing the input value by four.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToQuarter, num,
        num * 0.25f,
        num * 0.25);

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
        num * 0.01f,
        num * 0.01);

    /**
     * @brief Converts degrees to radians.
     * @tparam T The type of the input parameter.
     * @param deg A reference to the input parameter in degrees.
     * @return The input value converted to radians.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToRadians, deg,
        deg * (constant::fPI / 180.f),
        deg * (constant::dPI / 180.0));
    /**
     * @brief Converts radians to degrees.
     * @tparam T The type of the input parameter.
     * @param rad A reference to the input parameter in radians.
     * @return The input value converted to degrees.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(ToDegrees, rad,
        rad * (180.f / constant::fPI),
        rad * (180.0 / constant::dPI));

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
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(MSToS, t,
        t * 0.001f,
        t * 0.001);

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
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(USToMS, t,
        t * 0.001f,
        t * 0.001);
    /**
     * Converts a value in microseconds to seconds.
     * @tparam T The type of the input value.
     * @param t The input value in microseconds.
     * @return The input value in seconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(USToS, t,
        t * 0.000001f,
        t * 0.000001);

    // NS to
    /**
     * Converts a value in nanoseconds to microseconds.
     * @tparam T The type of the input value.
     * @param t The input value in nanoseconds.
     * @return The input value in microseconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToUS, t,
        t * 0.001f,
        t * 0.001);
    /**
     * Converts a value in nanoseconds to milliseconds.
     * @tparam T The type of the input value.
     * @param t The input value in nanoseconds.
     * @return The input value in milliseconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToMS, t,
        t * 0.000001f,
        t * 0.000001);
    /**
     * Converts a value in nanoseconds to seconds.
     * @tparam T The type of the input value.
     * @param t The input value in nanoseconds.
     * @return The input value in seconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToS, t,
        t * 0.000000001f,
        t * 0.000000001);

    /**
     * @brief Converts an iterable object to a container of the specified type, preserving the value types of the elements.
     *
     * This function takes an iterable object and returns a container of the specified type, where each element of the original
     * iterable is copied into the new container. The new container will have the same value type as the original iterable.
     *
     * @tparam Container A container type, such as std::vector or std::set.
     * @tparam T The type of the iterable object.
     * @param iterable The iterable object to convert to a container.
     * @return A new container containing all the elements from the iterable object.
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
     * @brief Converts the elements of an iterable object and inserts them into a new container type.
     *
     * This function iterates over the elements of an iterable object and applies the provided
     * conversion function to each element, converting it to the desired type. The resulting elements
     * are then inserted into a new container of the desired type, which is returned by the function.
     *
     * @tparam Container A template representing the type of container to store the converted elements.
     * @tparam ConvertFunc A template function to convert each element of the input iterable object to
     *                     the desired type.
     * @tparam T The type of the input iterable object.
     * @param iterable The iterable object to be converted.
     * @param convert_func The conversion function to apply to each element of the iterable object.
     * @return A new container of the specified type that contains the converted elements.
     */
    template<template<class> class Container, class ConvertFunc, class T>
    inline auto ToConvertedContainer(const T& iterable, ConvertFunc&& convert_func) {
        Container<decltype(convert_func(*iterable.begin()))> container;
        for (auto&& e : iterable) {
            container.insert(container.end(), convert_func(e));
        }
        return container;
    }

    /**
     * @brief Enumerate the elements in an iterable container.
     *
     * This function takes an iterable container and returns an iterable wrapper around
     * it that yields tuples of the form (index, element). This is useful for iterating over
     * containers and keeping track of the current index in the iteration.
     *
     * @tparam T The type of the iterable container.
     * @tparam Iter The type of the iterator for the container. This is determined
     *              automatically by the function based on the type of the container.
     * @param iterable The iterable container to enumerate.
     * @return An iterable wrapper that yields tuples of the form (index, element).
     */
    template<class T, class Iter = decltype(std::begin(std::declval<T>()))>
    inline auto Enumerate(T&& iterable) {
        // Define a custom iterator for the container that yields tuples of the form (index, element).
        struct Iterator {
            size_t i;  // The current index in the iteration.
            Iter iter; // The underlying iterator for the container.
            bool operator!=(const Iterator& other) const { return iter != other.iter; }
            void operator++()                            { ++i; ++iter; }
            auto operator*()                       const { return std::tuple(i, *iter); }
        };

        // Define a wrapper around the iterable container that returns the custom iterator.
        struct IterableWrapper {
            T iterable; // The iterable container to enumerate.
            auto begin() { return Iterator{ 0, std::begin(iterable) }; }
            auto end()   { return Iterator{ 0, std::end(iterable) }; }
        };

        // Return the iterable wrapper.
        return IterableWrapper{ std::forward<T>(iterable) };
    }

    /**
     * @brief Returns the size of the given argument as a `CastTy` type.
     *
     * This function returns the size of the given argument, which is passed by reference, as a `CastTy` type.
     * The `sizeof` operator returns the size of the argument in bytes, which is then cast to the desired `CastTy` type.
     * The result of the cast is returned.
     *
     * @tparam CastTy The type to cast the size of the argument to.
     * @tparam T The type of the argument.
     * @param arg The argument to get the size of.
     * @return The size of the argument as a `CastTy`.
     */
    template<class CastTy, class T>
    constexpr CastTy SizeOf(const T& arg) {
        return static_cast<CastTy>(sizeof(arg));
    }

    /**
     * @brief Normalize a given value to a specified range.
     *
     * This function takes a value `x` and normalizes it to the range specified by `x_min`
     * and `x_max`. The normalized value is then scaled to the range specified by `y_start`
     * and `y_end`.
     *
     * @param x The value to be normalized.
     * @param x_min The minimum value of the range to normalize `x` to.
     * @param x_max The maximum value of the range to normalize `x` to.
     * @param y_start The starting value of the output range.
     * @param y_end The ending value of the output range.
     * @return The normalized value of `x` in the specified output range.
     */
    inline float Normalize(float x, float x_min, float x_max, float y_start = 0.f, float y_end = 1.f) {
        // If x_min is not zero, shift x, x_min, and x_max so that x_min becomes zero.
        if (x_min != 0.f) {
            x -= x_min;
            x_max -= x_min;
            x_min = 0.f;
        }

        // If y_start is not zero, scale the output range accordingly.
        if (y_start != 0.f) {
            y_end -= y_start;
        }

        // Normalize x to the range [0, 1].
        float y = x / x_max;

        // Scale the normalized value to the output range.
        return (y_end * y) + y_start;
    }

}

#endif
