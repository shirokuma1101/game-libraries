/**
 * @file Macro.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-12
 *
 * @copyright Copyright (c) 2022 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_UTILITY_MACRO_H_
#define GAME_LIBRARIES_UTILITY_MACRO_H_

/* CPP versions */
#define MACRO_CPP98 199711L
#define MACRO_CPP11 201103L
#define MACRO_CPP14 201402L
#define MACRO_CPP17 201703L
#define MACRO_CPP20 202002L
#define MACRO_CPP23 202399L // 2023XXL

/* Pointer validity */
#define MACRO_VALID_POINTER(p, func)   p = func; p
#define MACRO_INVALID_POINTER(p, func) p = func; !p

/* Success check if the return value of the function is 0 and success */
#define MACRO_SUCCESS_CHECK(func, err_code) int err_code = func; !err_code
#define MACRO_FAIL_CHECK(func, err_code)    int err_code = func; err_code

/* Disable copy constructor */
#define MACRO_DISABLE_COPY_CONSTRUCTOR(class_name) \
    class_name(const class_name&) = delete;        \
    class_name& operator=(const class_name&) = delete

/* Getter */
#define MACRO_GETTER(class_name, getter_func_name, member_name) \
    virtual class_name getter_func_name() const noexcept final { return member_name; }

/* Const and non-const pointer getter */
#define MACRO_GETTER_PTR(class_name, getter_func_name, member_name)                           \
    virtual       class_name* getter_func_name()       noexcept final { return member_name; } \
    virtual const class_name* getter_func_name() const noexcept final { return member_name; }

/* Const and non-const reference getter */
#define MACRO_GETTER_REF(class_name, getter_func_name, member_name)                           \
    virtual       class_name& getter_func_name()       noexcept final { return member_name; } \
    virtual const class_name& getter_func_name() const noexcept final { return member_name; }

/* Const and non-const shared_ptr getter */
#define MACRO_GETTER_SHARED_PTR(class_name, getter_func_name, member_name)                      \
    std::shared_ptr<class_name>       getter_func_name()       noexcept { return member_name; } \
    std::shared_ptr<const class_name> getter_func_name() const noexcept { return member_name; }

/* Internal namespace */
#define MACRO_NAMESPACE_EXTERNAL_BEGIN namespace detail {} namespace {
#define MACRO_NAMESPACE_EXTERNAL_END   }
#define MACRO_NAMESPACE_INTERNAL_BEGIN namespace detail {
#define MACRO_NAMESPACE_INTERNAL_END   }

/* Enum class operator override */
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_AND(class_name)                     \
inline constexpr class_name operator&(class_name lhs, class_name rhs) noexcept { \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    return static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) &           \
                                   static_cast<UnderlyingTypeT>(rhs));           \
}
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_OR(class_name)                      \
inline constexpr class_name operator|(class_name lhs, class_name rhs) noexcept { \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    return static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) |           \
                                   static_cast<UnderlyingTypeT>(rhs));           \
}
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_XOR(class_name)                     \
inline constexpr class_name operator^(class_name lhs, class_name rhs) noexcept { \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    return static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) ^           \
                                   static_cast<UnderlyingTypeT>(rhs));           \
}
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_NOT(class_name)                     \
inline constexpr class_name operator~(class_name lhs) noexcept {                 \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    return static_cast<class_name>(~static_cast<UnderlyingTypeT>(lhs));          \
}
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_AND_ASSIGN(class_name)              \
inline class_name& operator&=(class_name& lhs, class_name rhs) noexcept {        \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    lhs = static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) &            \
                                  static_cast<UnderlyingTypeT>(rhs));            \
    return lhs;                                                                  \
}
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_OR_ASSIGN(class_name)               \
inline class_name& operator|=(class_name& lhs, class_name rhs) noexcept {        \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    lhs = static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) |            \
                                  static_cast<UnderlyingTypeT>(rhs));            \
    return lhs;                                                                  \
}
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_XOR_ASSIGN(class_name)              \
inline class_name& operator^=(class_name& lhs, class_name rhs) noexcept {        \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    lhs = static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) ^            \
                                  static_cast<UnderlyingTypeT>(rhs));            \
    return lhs;                                                                  \
}
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE(class_name)                         \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_AND(class_name)                             \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_OR(class_name)                              \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_XOR(class_name)                             \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_NOT(class_name)                             \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_AND_ASSIGN(class_name)                      \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_OR_ASSIGN(class_name)                       \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_XOR_ASSIGN(class_name)

/* Singleton */
#define MACRO_SIMPLE_SINGLETON(class_name, func_name) \
private:                                              \
    class_name() {}                                   \
public:                                               \
    static class_name& func_name() noexcept {         \
        static class_name instance;                   \
        return instance;                              \
    }
#define MACRO_SINGLETON(class_name, work_instance_func_name, instance_func_name, destruct_func_name) \
private:                                                                                             \
    class_name() {}                                                                                  \
    inline static class_name* instance;                                                              \
public:                                                                                              \
    static class_name& work_instance_func_name() noexcept {                                          \
        if (!instance) {                                                                             \
            instance = new class_name;                                                               \
            atexit(&destruct_func_name);                                                             \
        }                                                                                            \
        return *instance;                                                                            \
    }                                                                                                \
    static const class_name& instance_func_name() noexcept {                                         \
        if (!instance) {                                                                             \
            instance = new class_name;                                                               \
            atexit(&destruct_func_name);                                                             \
        }                                                                                            \
        return *instance;                                                                            \
    }                                                                                                \
    static void destruct_func_name() {                                                               \
        if (!instance) return;                                                                       \
        delete instance;                                                                             \
        instance = nullptr;                                                                          \
        return;                                                                                      \
    }

#endif
