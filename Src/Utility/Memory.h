#pragma once

namespace memory {

    template<class T>
    inline void SafeRelease(T* p) {
        if (!p) return;
        p->Release();
        p = nullptr;
    }

    template<class T>
    inline void SafeRelease(T* p) {
        if (!p) return;
        p->release();
        p = nullptr;
    }

    template<class T>
    inline void SafeDelete(T* p) {
        if (!p) return;
        delete p;
        p = nullptr;
    }

}
