/**
 * @file Macro.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_UTILITY_MACRO_H_
#define GAME_LIBRARIES_UTILITY_MACRO_H_

/**
 * @brief Macro to check version of C++.
 * @note If using visual studio, C/C++ -> command line -> Additional options -> /Zc:__cplusplus
 */
#define CPP_VERSION __cplusplus

/**
 * @brief Macro to check if the compiler is C++98.
 */
#define MACRO_CPP98 199711L
/**
 * @brief Macro to check if the compiler is C++11.
 */
#define MACRO_CPP11 201103L
/**
 * @brief Macro to check if the compiler is C++14.
 */
#define MACRO_CPP14 201402L
/**
 * @brief Macro to check if the compiler is C++17.
 */
#define MACRO_CPP17 201703L
/**
 * @brief Macro to check if the compiler is C++20.
 */
#define MACRO_CPP20 202002L
/**
 * @brief Macro to check if the compiler is C++23.
 */
#define MACRO_CPP23 202399L // 2023XXL

/**
 * @brief Macro to check if a pointer is valid, assigning the result of a function to it.
 * @param p The pointer to check and assign.
 * @param func The function to check the pointer against.
 * @return A boolean indicating if the pointer is invalid after assignment.
 */
#define MACRO_VALID_POINTER(p, func)   p = func; p
/**
 * @brief Macro to check if a pointer is invalid, assigning the result of a function to it.
 * @param p The pointer to check and assign.
 * @param func The function to check the pointer against.
 * @return A boolean indicating if the pointer is invalid after assignment.
 */
#define MACRO_INVALID_POINTER(p, func) p = func; !p

/**
 * @brief Macro to check if a function returns 0, assigning the result of the function to an error code.
 * @param func The function to check.
 * @param err_code The error code to assign the result of the function to.
 * @return A boolean indicating if the function returned 0.
 */
#define MACRO_SUCCESS_CHECK(func, err_code) int err_code = func; !err_code
/**
 * @brief Macro to check if a function returns 0, assigning the result of the function to an error code.
 * @param func The function to check.
 * @param err_code The error code to assign the result of the function to.
 * @return A boolean indicating if the function returned 0.
 */
#define MACRO_FAIL_CHECK(func, err_code)    int err_code = func; err_code

/**
 * @brief Macro to disable the copy constructor and assignment operator.
 * @param class_name The name of the class to disable the copy constructor and assignment operator for.
 */
#define MACRO_DISABLE_COPY_CONSTRUCTOR(class_name) \
    class_name(const class_name&) = delete;        \
    class_name& operator=(const class_name&) = delete

/**
 * @brief Macro to create a const getter.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return The member variable.
 */
#define MACRO_GETTER_AS_IS(class_name, getter_func_name, member_name) \
    virtual class_name getter_func_name() const noexcept final { return member_name; }

/**
 * @brief Macro to create a pointer getter that returns the pointer as-is.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A pointer to the member variable.
 */
#define MACRO_GETTER_PTR_AS_IS(class_name, getter_func_name, member_name) \
    virtual       class_name* getter_func_name()       noexcept final { return member_name; }
/**
 * @brief Macro to create a const pointer getter.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A const pointer to the member variable.
 */
#define MACRO_GETTER_CONST_PTR(class_name, getter_func_name, member_name) \
    virtual const class_name* getter_func_name() const noexcept final { return member_name; }
/**
 * @brief Macro to create a const and non-const pointer getter.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A pointer to the member variable.
 */
#define MACRO_GETTER_PTR(class_name, getter_func_name, member_name)   \
    MACRO_GETTER_PTR_AS_IS(class_name, getter_func_name, member_name) \
    MACRO_GETTER_CONST_PTR(class_name, getter_func_name, member_name)

/**
 * @brief Macro to create a pointer address getter that returns the pointer address as-is.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A pointer to the pointer to the member variable.
 */
#define MACRO_GETTER_PTR_ADDR_AS_IS(class_name, getter_func_name, member_name) \
    virtual       class_name**       getter_func_name()       noexcept final { return member_name; }
/**
 * @brief Macro to create a const pointer address getter.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A const pointer to the pointer to the member variable.
 */
#define MACRO_GETTER_PTR_CONST_ADDR(class_name, getter_func_name, member_name) \
    virtual const class_name* const* getter_func_name() const noexcept final { return member_name; }
/**
 * @brief Macro to create a const and non-const pointer address getter.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A pointer to the pointer to the member variable.
 */
#define MACRO_GETTER_PTR_ADDR(class_name, getter_func_name, member_name)   \
    MACRO_GETTER_PTR_ADDR_AS_IS(class_name, getter_func_name, member_name) \
    MACRO_GETTER_PTR_CONST_ADDR(class_name, getter_func_name, member_name)

/**
 * @brief Macro to create a reference getter that returns the reference as-is.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A reference to the member variable.
 */
#define MACRO_GETTER_REF_AS_IS(class_name, getter_func_name, member_name) \
    virtual       class_name& getter_func_name()       noexcept final { return member_name; }
/**
 * @brief Macro to create a const reference getter.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A const reference to the member variable.
 */
#define MACRO_GETTER_CONST_REF(class_name, getter_func_name, member_name) \
    virtual const class_name& getter_func_name() const noexcept final { return member_name; }
/**
 * @brief Macro to create a const and non-const reference getter.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A reference to the member variable.
 */
#define MACRO_GETTER_REF(class_name, getter_func_name, member_name)   \
    MACRO_GETTER_REF_AS_IS(class_name, getter_func_name, member_name) \
    MACRO_GETTER_CONST_REF(class_name, getter_func_name, member_name)

/**
 * @brief Macro to create a shared pointer getter that returns the shared pointer as-is.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A shared pointer to the member variable.
 */
#define MACRO_GETTER_SHARED_PTR_AS_IS(class_name, getter_func_name, member_name) \
    virtual std::shared_ptr<class_name>       getter_func_name()       noexcept final { return member_name; }
/**
 * @brief Macro to create a const shared pointer getter.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A const shared pointer to the member variable.
 */
#define MACRO_GETTER_CONST_SHARED_PTR(class_name, getter_func_name, member_name) \
    virtual std::shared_ptr<const class_name> getter_func_name() const noexcept final { return member_name; }
/**
 * @brief Macro to create a const and non-const shared pointer getter.
 * @param class_name The name of the class to create the getter for.
 * @param getter_func_name The name of the getter function.
 * @param member_name The name of the member variable to get.
 * @return A shared pointer to the member variable.
 */
#define MACRO_GETTER_SHARED_PTR(class_name, getter_func_name, member_name)   \
    MACRO_GETTER_SHARED_PTR_AS_IS(class_name, getter_func_name, member_name) \
    MACRO_GETTER_CONST_SHARED_PTR(class_name, getter_func_name, member_name)

/**
 * @brief Macro to begin an external namespace.
 */
#define MACRO_NAMESPACE_EXTERNAL_BEGIN namespace detail {} namespace {
/**
 * @brief Macro to end an external namespace.
 */
#define MACRO_NAMESPACE_EXTERNAL_END   }
/**
 * @brief Macro to begin an internal namespace.
 */
#define MACRO_NAMESPACE_INTERNAL_BEGIN namespace detail {
/**
 * @brief Macro to end an internal namespace.
 */
#define MACRO_NAMESPACE_INTERNAL_END   }

/**
 * @brief Macro to create a bitwise and operator overload.
 * @param class_name The name of the class to create the operator overload for.
 */
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_AND(class_name)                     \
inline constexpr class_name operator&(class_name lhs, class_name rhs) noexcept { \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    return static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) &           \
                                   static_cast<UnderlyingTypeT>(rhs));           \
}
/**
 * @brief Macro to create a bitwise or operator overload.
 * @param class_name The name of the class to create the operator overload for.
 */
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_OR(class_name)                      \
inline constexpr class_name operator|(class_name lhs, class_name rhs) noexcept { \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    return static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) |           \
                                   static_cast<UnderlyingTypeT>(rhs));           \
}
/**
 * @brief Macro to create a bitwise xor operator overload.
 * @param class_name The name of the class to create the operator overload for.
 */
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_XOR(class_name)                     \
inline constexpr class_name operator^(class_name lhs, class_name rhs) noexcept { \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    return static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) ^           \
                                   static_cast<UnderlyingTypeT>(rhs));           \
}
/**
 * @brief Macro to create a bitwise not operator overload.
 * @param class_name The name of the class to create the operator overload for.
 */
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_NOT(class_name)                     \
inline constexpr class_name operator~(class_name lhs) noexcept {                 \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    return static_cast<class_name>(~static_cast<UnderlyingTypeT>(lhs));          \
}
/**
 * @brief Macro to create a bitwise and assignment operator overload.
 * @param class_name The name of the class to create the operator overload for.
 */
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_AND_ASSIGN(class_name)              \
inline class_name& operator&=(class_name& lhs, class_name rhs) noexcept {        \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    lhs = static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) &            \
                                  static_cast<UnderlyingTypeT>(rhs));            \
    return lhs;                                                                  \
}
/**
 * @brief Macro to create a bitwise or assignment operator overload.
 * @param class_name The name of the class to create the operator overload for.
 */
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_OR_ASSIGN(class_name)               \
inline class_name& operator|=(class_name& lhs, class_name rhs) noexcept {        \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    lhs = static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) |            \
                                  static_cast<UnderlyingTypeT>(rhs));            \
    return lhs;                                                                  \
}
/**
 * @brief Macro to create a bitwise xor assignment operator overload.
 * @param class_name The name of the class to create the operator overload for.
 */
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_XOR_ASSIGN(class_name)              \
inline class_name& operator^=(class_name& lhs, class_name rhs) noexcept {        \
    using UnderlyingTypeT = templates::UnderlyingTypeWrapperT<class_name>;       \
    lhs = static_cast<class_name>(static_cast<UnderlyingTypeT>(lhs) ^            \
                                  static_cast<UnderlyingTypeT>(rhs));            \
    return lhs;                                                                  \
}
/**
 * @brief Macro to overload bitwise operators for an enum class.
 * @param class_name The name of the class to create the operator overload for.
 */
#define MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE(class_name)                         \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_AND(class_name)                             \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_OR(class_name)                              \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_XOR(class_name)                             \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_NOT(class_name)                             \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_AND_ASSIGN(class_name)                      \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_OR_ASSIGN(class_name)                       \
MACRO_ENUM_OPERATOR_OVERLOAD_BITWISE_XOR_ASSIGN(class_name)

/**
 * @brief Simple singleton macro
 * @param class_name Class name
 * @param func_name Function name
 */
#define MACRO_SIMPLE_SINGLETON(class_name, func_name) \
private:                                              \
    class_name() {}                                   \
public:                                               \
    static class_name& func_name() noexcept {         \
        static class_name instance;                   \
        return instance;                              \
    }
/**
 * @brief Singleton macro with work instance
 * @param class_name Class name
 * @param work_instance_func_name Work instance function name
 * @param instance_func_name Instance function name
 * @param destruct_func_name Destruct function name
 */
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
