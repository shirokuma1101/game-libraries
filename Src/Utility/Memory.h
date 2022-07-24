#pragma once

namespace memory {

    template<class T>
    void SafeRelease(T* p) {
        if (!p) return;
        p->Release();
        p = nullptr;
    }

    template<class T>
    void SafeDelete(T* p) {
        if (!p) return;
        delete p;
        p = nullptr;
    }

}
