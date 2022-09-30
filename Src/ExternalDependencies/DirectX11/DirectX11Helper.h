#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11HELPER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11HELPER_H_

#include <d3d11.h>

#include "Utility/Memory.h"

namespace directx11_helper {

    inline D3D11_TEXTURE2D_DESC GetTextureInfo(ID3D11View* view) {
        D3D11_TEXTURE2D_DESC td{};
        ID3D11Resource* resource = nullptr;
        ID3D11Texture2D* texture2d = nullptr;
        
        memset(&td, 0, sizeof(td));
        view->GetResource(&resource);
        if (SUCCEEDED(resource->QueryInterface<ID3D11Texture2D>(&texture2d))) {
            texture2d->GetDesc(&td);
            memory::SafeRelease(&texture2d);
        }
        memory::SafeRelease(&resource);
    }

}

#endif
