#pragma once

#ifndef GAME_LIBRARIES_UTILITY_MACRO_H_
#define GAME_LIBRARIES_UTILITY_MACRO_H_

#define CPP98 199711L
#define CPP11 201103L
#define CPP14 201402L
#define CPP17 201703L
#define CPP20 202002L
#define CPP23 202399L // 2023XXL

#define NAMESPACE_EXTERNAL_BEGIN namespace detail {} namespace {
#define NAMESPACE_EXTERNAL_END   }
#define NAMESPACE_INTERNAL_BEGIN namespace detail {
#define NAMESPACE_INTERNAL_END   }

#define SINGLETON(class_name, func_name) \
private:                                 \
    class_name() {}                      \
public:                                  \
    static class_name& func_name() {     \
        static class_name instance;      \
        return instance;                 \
    }

#define VALID_POINTER(p, func)   p = func; p
#define INVALID_POINTER(p, func) p = func; !p

#define FAIL_CHECK(func, err_code)    int err_code = func; err_code
#define SUCCESS_CHECK(func, err_code) int err_code = func; !err_code

#endif
