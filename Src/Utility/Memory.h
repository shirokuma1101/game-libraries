#pragma once

#ifndef GAME_LIBRARIES_UTILITY_MEMORY_H_
#define GAME_LIBRARIES_UTILITY_MEMORY_H_

#include "Utility/Templates.h"

/**************************************************
* 
* Memory
* 
**************************************************/
namespace memory {
    
    template<class T>
    inline void SafeRelease(T** p) {
        if (!*p) return;

        if constexpr (TEMPLATES_HAS_FUNC(T, Release())) {
            (*p)->Release();
        }
        else if constexpr (TEMPLATES_HAS_FUNC(T, release())) {
            (*p)->release();
        }
        else {
            static_assert(templates::false_v<T>, "No 'Release' or 'release' function defined");
        }

        *p = nullptr;
    }
    
    template<class T>
    inline void SafeDelete(T** p) {
        if (!*p) return;
        delete *p;
        *p = nullptr;
    }

}

#endif
