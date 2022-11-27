#pragma once

#ifndef GAME_LIBRARIES_UTILITY_TEMPLATES_H_
#define GAME_LIBRARIES_UTILITY_TEMPLATES_H_

#include <type_traits>

namespace templates {

    template<class>
    constexpr bool true_v = true;
    template<class>
    constexpr bool false_v = false;

    /* Function to check if the target function is defined in the specified class */
    template<class T, class Func>
    constexpr auto HasFunc(Func&& f) noexcept ->decltype(f(std::declval<T>()), true) {
        return true;
    }
    template<class>
    constexpr bool HasFunc(...) noexcept {
        return false;
    }
    // Macros to simplify access to HasFunc
#define TEMPLATES_HAS_FUNC(type, func) templates::HasFunc<type>([](auto&& obj)->decltype(obj.func){})

    /* Possible to cast */
    template<class T, class U>
    constexpr auto IsCastable() noexcept ->decltype(static_cast<U>(std::declval<T>())) {
        return true;
    }
    template<class>
    constexpr bool IsCastable(...) noexcept {
        return false;
    }

    //note: "https://cpprefjp.github.io/reference/type_traits/underlying_type.html"
    template<class T, bool is_enum>
    struct UnderlyingTypeWrapperImpl : std::add_const<T> {};
    template<class T>
    struct UnderlyingTypeWrapperImpl<T, true> : std::underlying_type<T> {};
    template<class T>
    struct UnderlyingTypeWrapper : UnderlyingTypeWrapperImpl<T, std::is_enum<T>::value> {};
    template<class T>
    using UnderlyingTypeWrapperT = typename UnderlyingTypeWrapper<T>::type;
    
}

#endif
