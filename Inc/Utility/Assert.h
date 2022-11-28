#pragma once

#ifndef GAME_LIBRARIES_UTILITY_ASSERT_H_
#define GAME_LIBRARIES_UTILITY_ASSERT_H_

#include <crtdbg.h>
#include <stdexcept>
#include <string_view>

/**************************************************
* 
* Show info, warning, error in visual studio output
* 
**************************************************/
namespace assert {
    
#define ASSERT_FILE_LINE __FILE__, __LINE__

#ifdef _DEBUG
    inline void ShowInfo(char const* file, int line, std::string_view message) {
        _CrtDbgReport(_CRT_WARN, file, line, NULL, "info: %s\n", message.data());
    }
    inline void ShowInfo(std::string_view message) {
        ShowInfo(ASSERT_FILE_LINE, message);
    }
    inline void ShowWarning(char const* file, int line, std::string_view message) {
        _CrtDbgReport(_CRT_WARN, file, line, NULL, "warning: %s\n", message.data());
    }
    inline void ShowWarning(std::string_view message) {
        ShowWarning(ASSERT_FILE_LINE, message);
    }
    inline void ShowError(char const* file, int line, std::string_view message) {
        _CrtDbgReport(_CRT_ERROR, file, line, NULL, "error: %s\n", message.data());
    }
    inline void ShowError(std::string_view message) {
        ShowError(ASSERT_FILE_LINE, message);
    }
#else
    inline void ShowInfo(char const*, int = 0, std::string_view = "") {}
    inline void ShowInfo(std::string_view = "") {}
    inline void ShowWarning(char const*, int = 0, std::string_view = "") {}
    inline void ShowWarning(std::string_view = "") {}
    inline void ShowError(char const*, int = 0, std::string_view = "") {}
    inline void ShowError(std::string_view = "") {}
#endif // _DEBUG
    inline void ExceptionThrow(std::string_view message) {
        throw std::runtime_error(message.data());
    }

}

#endif
