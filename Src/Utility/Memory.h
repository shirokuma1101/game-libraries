#pragma once

namespace memory {

    template<class T>
    inline auto SafeRelease(T* p)->decltype(p->Release(), void()) {
        if (!p) return;
        p->Release();
        p = nullptr;
    }

    template<class T>
    inline auto SafeRelease(T* p)->decltype(p->release(), void()) {
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
