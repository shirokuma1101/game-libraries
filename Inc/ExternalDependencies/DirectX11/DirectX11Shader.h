#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11SHADER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11SHADER_H_

#include "Utility/Assert.h"
#include "Utility/Memory.h"
#include "DirectX11Helper.h"

template<class T>
class DirectX11Shader
{
public:

    DirectX11Shader(ID3D11Device* dev)
        : m_pDev(dev)
    {}
    virtual ~DirectX11Shader() {
        Release();
    }

protected:

    ID3D11Device* m_pDev = nullptr;

};

#endif
