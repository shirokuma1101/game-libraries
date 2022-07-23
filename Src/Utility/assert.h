#pragma once

#include <string_view>
#include <crtdbg.h>

namespace assert {

    inline void ShowWarning(std::string_view message) {
        _CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, "warning: %s\n", message.data());
    }

    inline void RaiseAssert(std::string_view message) {
        _CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, NULL, "error: %s\n", message.data());
    }

    inline void ExceptionThrow(std::string_view message) {
        throw std::exception(message.data());
    }

}
