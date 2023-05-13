#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11INPUTLAYOUT_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11INPUTLAYOUT_H_

#include <d3d11_4.h>
#include <memory>

#include "Utility/Assert.h"
#include "Utility/Macro.h"
#include "Utility/Memory.h"
#include "DirectX11Helper.h"


class DirectX11InputLayout
{
public:

    DirectX11InputLayout(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_pDev(dev)
        , m_pCtx(ctx)
    {}
    virtual ~DirectX11InputLayout() {
        Release();
    }
    
    MACRO_GETTER_PTR(ID3D11InputLayout, Get, m_pInputLayout);

    void SetUpInputElementDesc(const std::vector<std::tuple<std::string_view, UINT, DXGI_FORMAT>>& input_layout) noexcept {
        for (const auto& e : input_layout) {
            m_inputElementDesc.push_back({
                std::get<0>(e).data(),        // SemanticName
                std::get<1>(e),               // SemanticIndex
                std::get<2>(e),               // Format
                0,                            // InputSlot
                D3D11_APPEND_ALIGNED_ELEMENT, // AlignedByteOffset
                D3D11_INPUT_PER_VERTEX_DATA , // InputSlotClass
                0                             // InstanceDataStepRate
            });
        }
    }
    
    bool Create(ID3DBlob* blob) {
        if (FAILED(m_pDev->CreateInputLayout(&m_inputElementDesc[0], static_cast<UINT>(m_inputElementDesc.size()), blob->GetBufferPointer(), blob->GetBufferSize(), &m_pInputLayout))) {
            assert::ShowError(ASSERT_FILE_LINE, "input layout create failed");
            return false;
        }
        return true;
    }

    void Set() {
        m_pCtx->IASetInputLayout(m_pInputLayout);
    }

protected:

    ID3D11Device*        m_pDev = nullptr;
    ID3D11DeviceContext* m_pCtx = nullptr;

    ID3D11InputLayout*                    m_pInputLayout = nullptr;
    std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputElementDesc;

private:

    void Release() {
        memory::SafeRelease(&m_pInputLayout);
    }

};

#endif
