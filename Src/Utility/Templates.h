#pragma once

#ifndef GAME_LIBRARIES_UTILITY_TEMPLATES_H_
#define GAME_LIBRARIES_UTILITY_TEMPLATES_H_

#include <type_traits>

namespace templates {

    template <class T>
    constexpr bool true_v = false;
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

}

#endif
