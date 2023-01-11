/**
 * @file Templates.h
 * @author shirokuma1101
 * @version 1.0
 * @date 2022-12-18
 *
 * @copyright Copyright (c) 2022 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_UTILITY_TEMPLATES_H_
#define GAME_LIBRARIES_UTILITY_TEMPLATES_H_

#include <type_traits>

/**
 * @brief A namespace containing various templates.
 */
namespace templates {

    /**
     * @brief A boolean constant with the value true, specialized for a given type.
     * @tparam T The type to specialize for.
     */
    template<class>
    constexpr bool true_v = true;
    /**
     * @brief A boolean constant with the value false, specialized for a given type.
     * @tparam T The type to specialize for.
     */
    template<class>
    constexpr bool false_v = false;

    /**
     * @brief Determines if the specified type has a member function with the specified name.
     * @tparam T The type to check for the member function.
     * @tparam Func The type of the member function.
     * @param f The member function to check for.
     * @return true if the type has the member function, false otherwise.
     */
    template<class T, class Func>
    constexpr auto HasFunc(Func&& f) noexcept ->decltype(f(std::declval<T>()), true) {
        return true;
    }
    /**
     * @brief Overload of HasFunc that returns false.
     * @tparam T Unused template parameter.
     * @return false.
     */
    template<class>
    constexpr bool HasFunc(...) noexcept {
        return false;
    }
    /**
     * @brief Macro to simplify access to the templates::HasFunc function.
     * @param type The type to check for the member function.
     * @param func The name of the member function to check for.
     */
    #define TEMPLATES_HAS_FUNC(type, func) templates::HasFunc<type>([](auto&& obj)->decltype(obj.func){})

    /**
     * @brief Tests whether a object of type T can be cast to type U.
     * @tparam T The type to be cast from.
     * @tparam U The type to be cast to.
     * @return Returns true if the object of type T can be successfully cast to type U, false otherwise.
     */
    template<class T, class U>
    constexpr auto IsCastable() noexcept ->decltype(static_cast<U>(std::declval<T>())) {
        return true;
    }
    /**
     * @brief Tests whether a object of an unknown type can be cast.
     * @return Returns false if the object cannot be cast, true otherwise.
     */
    template<class>
    constexpr bool IsCastable(...) noexcept {
        return false;
    }

    /**
     * @brief A wrapper class for the underlying type of an enum type.
     * @tparam T The type to be wrapped.
     * @tparam is_enum A boolean value indicating whether T is an enum type.
     */
    template<class T, bool is_enum>
    struct UnderlyingTypeWrapperImpl : std::add_const<T> {};
    /**
     * @brief A specialization of UnderlyingTypeWrapperImpl for enum types.
     * @tparam T The type to be wrapped.
     */
    template<class T>
    struct UnderlyingTypeWrapperImpl<T, true> : std::underlying_type<T> {};
    /**
     * @brief A class that wraps the underlying type of an enum type.
     * @tparam T The type to be wrapped.
     */
    template<class T>
    struct UnderlyingTypeWrapper : UnderlyingTypeWrapperImpl<T, std::is_enum<T>::value> {};
    /**
     * @brief An alias for the wrapped underlying type.
     * @tparam T The type to be wrapped.
     */
    template<class T>
    using UnderlyingTypeWrapperT = typename UnderlyingTypeWrapper<T>::type;

}

#endif
