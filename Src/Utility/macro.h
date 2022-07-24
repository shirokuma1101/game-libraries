#pragma once

#define NAMESPACE_EXTERNAL       namespace detail {} namespace {
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
