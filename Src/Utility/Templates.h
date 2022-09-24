#pragma once

#ifndef GAME_LIBRARIES_UTILITY_TEMPLATES_H_
#define GAME_LIBRARIES_UTILITY_TEMPLATES_H_

#include <type_traits>

namespace templates {

    template <class T>
    constexpr bool true_v = true;
    template <class T>
    constexpr bool false_v = false;

#define TEMPLATES_HAS_FUNC(type, func) templates::HasFunc<type>([](auto&& obj)->decltype(obj.func){})
    template<class T, class Func>
    constexpr auto HasFunc(Func&& f)->decltype(f(std::declval<T>()), true) {
        return true;
    }
    template<class>
    constexpr bool HasFunc(...) {
        return false;
    }

    template<class T, class U>
    constexpr auto IsSafelyCastable()->decltype(static_cast<U>(std::declval<T>())) {
        return true;
    }
    template<class>
    constexpr bool IsSafelyCastable(...) {
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
