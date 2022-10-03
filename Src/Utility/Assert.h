#pragma once

#ifndef GAME_LIBRARIES_UTILITY_ASSERT_H_
#define GAME_LIBRARIES_UTILITY_ASSERT_H_

#include <crtdbg.h>
#include <string_view>

namespace assert {

#ifdef _DEBUG
    inline void ShowWarning(std::string_view message) {
        _CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, "warning: %s\n", message.data());
    }
    
    inline void RaiseAssert(std::string_view message) {
        _CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, NULL, "error: %s\n", message.data());
    }
#else
    inline void ShowWarning(std::string_view message) {
        message; // C4100 'identifier' : unreferenced formal parameter
    }
    inline void RaiseAssert(std::string_view message) {
        message; // C4100 'identifier' : unreferenced formal parameter
    }
#endif // _DEBUG
    
    inline void ExceptionThrow(std::string_view message) {
        throw std::exception(message.data());
    }

}

#endif
