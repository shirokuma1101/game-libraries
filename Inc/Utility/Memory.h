/**
 * @file Memory.h
 * @author shirokuma1101
 * @version 1.0
 * @date 2022-12-18
 *
 * @copyright Copyright (c) 2022 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_UTILITY_MEMORY_H_
#define GAME_LIBRARIES_UTILITY_MEMORY_H_

#include "Utility/Templates.h"

/**
 * @namespace memory
 * @brief Namespace containing functions for safely releasing and deleting objects.
 */
namespace memory {

    /**
     * @brief Safely release an object.
     * @tparam T Type of the object.
     * @param p Pointer to the object to release.
     */
    template<class T>
    inline void SafeRelease(T** p) {
        if (!*p) return;

        // Check if the object has a Release or release function.
        if constexpr (TEMPLATES_HAS_FUNC(T, Release())) {
            (*p)->Release();
        }
        else if constexpr (TEMPLATES_HAS_FUNC(T, release())) {
            (*p)->release();
        }
        // If neither function is defined, trigger a static_assert.
        else {
            static_assert(templates::false_v<T>, "No 'Release' or 'release' function defined");
        }

        *p = nullptr;
    }

    /**
     * @brief Safely delete an object.
     * @tparam T Type of the object.
     * @param p Pointer to the object to delete.
     */
    template<class T>
    inline void SafeDelete(T** p) {
        if (!*p) return;
        delete *p;
        *p = nullptr;
    }

}

#endif
