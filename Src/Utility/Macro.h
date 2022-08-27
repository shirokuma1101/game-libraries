#pragma once

#define NAMESPACE_EXTERNAL_BEGIN namespace detail {} namespace {
#define NAMESPACE_EXTERNAL_END   }
#define NAMESPACE_INTERNAL_BEGIN namespace detail {
#define NAMESPACE_INTERNAL_END   }

#define SINGLETON(class_name)          \
private:                               \
    class_name() {}                    \
public:                                \
    static class_name& GetInstance() { \
        static class_name Instance;    \
        return Instance;               \
    }

#define VALID_POINTER(p, func) p = func; p
#define INVALID_POINTER(p, func) p = func; !p

#define FAIL_CHECK(func, err) int err = func; err
#define SUCCESS_CHECK(func, err) int err = func; !err
