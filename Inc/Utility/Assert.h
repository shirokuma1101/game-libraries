#pragma once

#ifndef GAME_LIBRARIES_UTILITY_ASSERT_H_
#define GAME_LIBRARIES_UTILITY_ASSERT_H_

#include <crtdbg.h>
#include <stdexcept>
#include <string_view>

/**
 * @namespace assert
 * @brief Namespace containing functions for displaying messages in visual studio output only in debug builds and also for throwing exceptions
 */
namespace assert {

    /**
     * @def ASSERT_FILE_LINE
     * @brief Macro for the current file and line number
     */
    #define ASSERT_FILE_LINE __FILE__, __LINE__

#ifdef _DEBUG
    /**
     * @brief Show an info message
     * @param file File name
     * @param line Line number
     * @param message Message to display in the visual studio output
     */
    inline void ShowInfo(char const* file, int line, std::string_view message) {
        _CrtDbgReport(_CRT_WARN, file, line, NULL, "info: %s\n", message.data());
    }
    /**
     * @brief Show an info message
     * @param message Message to display in the visual studio output
     */
    inline void ShowInfo(std::string_view message) {
        ShowInfo(ASSERT_FILE_LINE, message);
    }
    /**
     * @brief Show a warning message
     * @param file File name
     * @param line Line number
     * @param message Message to display in the visual studio output
     */
    inline void ShowWarning(char const* file, int line, std::string_view message) {
        _CrtDbgReport(_CRT_WARN, file, line, NULL, "warning: %s\n", message.data());
    }
    /**
     * @brief Show a warning message
     * @param message Message to display in the visual studio output
     */
    inline void ShowWarning(std::string_view message) {
        ShowWarning(ASSERT_FILE_LINE, message);
    }
    /**
     * @brief Show an error message
     * @param file File name
     * @param line Line number
     * @param message Message to display in the visual studio output and in the popup window
     */
    inline void ShowError(char const* file, int line, std::string_view message) {
        _CrtDbgReport(_CRT_ERROR, file, line, NULL, "error: %s\n", message.data());
    }
    /**
     * @brief Show an error message
     * @param message Message to display in the visual studio output and in the popup window
     */
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
    /**
    * @brief Throws a runtime error with the specified message
    * @param message The message to include in the exception
    * @throw std::runtime_error The exception that is thrown
    */
    inline void ExceptionThrow(std::string_view message) {
        throw std::runtime_error(message.data());
    }

}

#endif
