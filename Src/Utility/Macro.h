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

/* Internal namespace */
#define MACRO_NAMESPACE_EXTERNAL_BEGIN namespace detail {} namespace {
#define MACRO_NAMESPACE_EXTERNAL_END   }
#define MACRO_NAMESPACE_INTERNAL_BEGIN namespace detail {
#define MACRO_NAMESPACE_INTERNAL_END   }

/* Enum class operator override */
#define MACRO_ENUM_CLASS_OPERATOR_OVERLOAD_AND(class_name)                        \
inline bool operator&(class_name lhs, class_name rhs) {                           \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;        \
    return static_cast<UnderlyingTypeT>(lhs) & static_cast<UnderlyingTypeT>(rhs); \
}
#define MACRO_ENUM_CLASS_OPERATOR_OVERLOAD_OR(class_name)                         \
inline bool operator|(class_name lhs, class_name rhs) {                           \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;        \
    return static_cast<UnderlyingTypeT>(lhs) | static_cast<UnderlyingTypeT>(rhs); \
}

/* Singleton */
#define MACRO_SIMPLE_SINGLETON(class_name, func_name) \
private:                                              \
    class_name() {}                                   \
public:                                               \
    static class_name& func_name() noexcept {         \
        static class_name instance;                   \
        return instance;                              \
    }
#define MACRO_SINGLETON(class_name, instance_func_name, destruct_func_name) \
private:                                                                    \
    class_name() {}                                                         \
    ~class_name() {}                                                        \
    static class_name* instance;                                            \
public:                                                                     \
    static class_name& instance_func_name() noexcept {                      \
        if (!instance) {                                                    \
            instance = new class_name;                                      \
            atexit(&destruct_func_name);                                    \
        }                                                                   \
        return *instance;                                                   \
    }                                                                       \
    static void destruct_func_name() {                                      \
        if (!instance) return;                                              \
        delete instance;                                                    \
        instance = nullptr;                                                 \
    }

#endif
