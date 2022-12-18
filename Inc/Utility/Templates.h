#pragma once

#ifndef GAME_LIBRARIES_UTILITY_TEMPLATES_H_
#define GAME_LIBRARIES_UTILITY_TEMPLATES_H_

#include <type_traits>

namespace templates {

    template<class>
    constexpr bool true_v = true;
    template<class>
    constexpr bool false_v = false;

    /**
     * @brief Determines if the specified type has a member function with the specified name
     * @tparam T The type to check for the member function
     * @tparam Func The type of the member function
     * @param f The member function to check for
     * @return true if the type has the member function, false otherwise
     */
    template<class T, class Func>
    constexpr auto HasFunc(Func&& f) noexcept ->decltype(f(std::declval<T>()), true) {
        return true;
    }
    /**
     * @brief Overload of HasFunc that returns false
     * @tparam T Unused template parameter
     * @return false
     */
    template<class>
    constexpr bool HasFunc(...) noexcept {
        return false;
    }
    /**
    * @brief Macro to simplify access to the templates::HasFunc function.
    *
    * @param type The type to check for the member function.
    * @param func The name of the member function to check for.
    */
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
