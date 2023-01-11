#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11BUFFER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11BUFFER_H_

#include <memory>
#include <d3d11.h>

#include "Utility/Assert.h"
#include "Utility/Macro.h"
#include "Utility/Memory.h"
#include "DirectX11Helper.h"


class DirectX11Buffer
{
public:

    DirectX11Buffer(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_pDev(dev)
        , m_pCtx(ctx)
    {
        if (!m_pDev || !m_pCtx) {
            assert::ShowError(ASSERT_FILE_LINE, "device or device context is nullptr.");
        }
    }
    virtual ~DirectX11Buffer() {
        Release();
    }

    ID3D11Buffer* Get() noexcept {
        return m_pBuffer;
    }
    ID3D11Buffer* Get() const noexcept {
        return m_pBuffer;
    }
    ID3D11Buffer* const* GetAddress() const noexcept {
        return &m_pBuffer;
    }
    UINT GetSize() const noexcept {
        return m_bufferSize;
    }

    bool Create(UINT buffer_size, D3D11_USAGE buffer_usage, D3D11_BIND_FLAG bind_flags, const D3D11_SUBRESOURCE_DATA* init_data) {
        Release();
        m_bufferSize  = buffer_size;
        m_bufferUsage = buffer_usage;
        return Create(m_pDev, &m_pBuffer, m_bufferSize, m_bufferUsage, bind_flags, init_data);
    }

    void Write(const void* src_data, UINT src_size) {
        Write(m_pCtx, m_pBuffer, m_bufferUsage, m_bufferSize, src_data, src_size);
    }

    void Set(directx11_helper::ShaderTarget target, UINT slot, UINT count = 1) {
        Set(m_pCtx, &m_pBuffer, target, slot, count);
    }

    void Release() {
        memory::SafeRelease(&m_pBuffer);
        m_bufferSize  = 0;
        m_bufferUsage = D3D11_USAGE_DEFAULT;
    }

    static bool Create(ID3D11Device* dev, ID3D11Buffer** buffer, UINT buffer_size, D3D11_USAGE buffer_usage, D3D11_BIND_FLAG bind_flags, const D3D11_SUBRESOURCE_DATA* init_data) {
        if (bind_flags & D3D11_BIND_CONSTANT_BUFFER) {
            if (buffer_size % 16 != 0) {
                assert::ShowError(ASSERT_FILE_LINE, "constant buffer size must be a multiple of 16 bytes.");
                return false;
            }
        }

        D3D11_BUFFER_DESC bd{};
        SecureZeroMemory(&bd, sizeof(bd));
        bd.ByteWidth           = buffer_size;
        bd.Usage               = buffer_usage;
        bd.BindFlags           = bind_flags;
        bd.MiscFlags           = 0;
        bd.StructureByteStride = 0;

        /* Static buffer */
        //  from CPU | Write × Read × (however, UpdateSubresource () is possible to update)
        //  from GPU | Write ○ Read ○
        // Video memory buffer Frequent updates are not suitable, but drawing is fast
        if (bd.Usage == D3D11_USAGE_DEFAULT) {
            bd.CPUAccessFlags = 0;
        }

        /* Dynamic buffer */
        //  from CPU | Write ○ Read ×
        //  from GPU | Write × Read ○
        // Buffers that are frequently updated are efficient, but the speed is a little slower than DEFAULT
        else if (bd.Usage == D3D11_USAGE_DYNAMIC) {
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }

        /* Staging buffer */
        //  from CPU | Write ○ Read ○
        //  from GPU | Write × Read ×
        // Direct3D can not be bound, but reading and writing transfer is possible for DEFAULT or DYNAMIC buffers
        // (Example) I want to get the contents of the DEFAULT buffer!
        //  -> Create a STAGING buffer, copy it from the DEFAULT buffer (CopyResource function). Then access the STAGING buffer (Map / Unmap).
        else if (bd.Usage == D3D11_USAGE_STAGING) {
            bd.BindFlags = 0;
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
        }

        if (FAILED(dev->CreateBuffer(&bd, init_data, buffer))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create buffer failed.");
            return false;
        }

        return true;
    }

    static void Write(ID3D11DeviceContext* ctx, ID3D11Buffer* buffer, D3D11_USAGE buffer_usage, UINT buffer_size, const void* src_data, UINT src_size) {
        if (buffer_usage == D3D11_USAGE_DEFAULT) {
            ctx->UpdateSubresource(buffer, 0, 0, src_data, 0, 0);
        }
        else if (buffer_usage == D3D11_USAGE_DYNAMIC) {
            D3D11_MAPPED_SUBRESOURCE ms{};
            if (SUCCEEDED(ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms))) {
                memcpy_s(ms.pData, buffer_size, src_data, src_size);
                ctx->Unmap(buffer, 0);
            }
        }
        else if (buffer_usage == D3D11_USAGE_STAGING) {
            D3D11_MAPPED_SUBRESOURCE ms{};
            if (SUCCEEDED(ctx->Map(buffer, 0, D3D11_MAP_READ_WRITE, 0, &ms))) {
                memcpy_s(ms.pData, buffer_size, src_data, src_size);
                ctx->Unmap(buffer, 0);
            }
        }
    }

    static void Set(ID3D11DeviceContext* ctx, ID3D11Buffer* const* buffer, directx11_helper::ShaderTarget target, UINT slot, UINT count) {
        switch (target) {
        case directx11_helper::ShaderTarget::CS:
            ctx->CSSetConstantBuffers(slot, count, buffer);
            break;
        case directx11_helper::ShaderTarget::DS:
            ctx->DSSetConstantBuffers(slot, count, buffer);
            break;
        case directx11_helper::ShaderTarget::GS:
            ctx->GSSetConstantBuffers(slot, count, buffer);
            break;
        case directx11_helper::ShaderTarget::HS:
            ctx->HSSetConstantBuffers(slot, count, buffer);
            break;
        case directx11_helper::ShaderTarget::PS:
            ctx->PSSetConstantBuffers(slot, count, buffer);
            break;
        case directx11_helper::ShaderTarget::VS:
            ctx->VSSetConstantBuffers(slot, count, buffer);
            break;
        }
    }

protected:

    ID3D11Device*        m_pDev        = nullptr;
    ID3D11DeviceContext* m_pCtx        = nullptr;
    ID3D11Buffer*        m_pBuffer     = nullptr;
    UINT                 m_bufferSize  = 0;
    D3D11_USAGE          m_bufferUsage = D3D11_USAGE_DEFAULT;

private:

    MACRO_DISABLE_COPY_CONSTRUCTOR(DirectX11Buffer);

};

template<class T>
class DirectX11ConstantBuffer
{
public:

    DirectX11ConstantBuffer(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_upBuffer(std::make_unique<DirectX11Buffer>(dev, ctx))
    {}
    virtual ~DirectX11ConstantBuffer() {
        Release();
    }

    T* Get() noexcept {
        m_isChanged = true;
        return &m_data;
    }
    const T* Get() const noexcept {
        return &m_data;
    }
    ID3D11Buffer* const* GetBufferAddress() const noexcept {
        return m_upBuffer->GetAddress();
    }

    bool Create() {
        D3D11_SUBRESOURCE_DATA sd{};
        SecureZeroMemory(&sd, sizeof(sd));
        sd.pSysMem          = &m_data;
        sd.SysMemPitch      = 0;
        sd.SysMemSlicePitch = 0;
        return m_upBuffer->Create(sizeof(m_data), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, &sd);
    }

    void Write(bool force_change = false) {
        if (!m_isChanged && !force_change) return;
        m_upBuffer->Write(&m_data, sizeof(m_data));
        m_isChanged = false;
    }

    void Set(directx11_helper::ShaderTarget target, UINT slot, UINT count = 1) {
        m_upBuffer->Set(target, slot, count);
    }

    void Release() {
        if (m_upBuffer) {
            m_upBuffer->Release();
            m_upBuffer = nullptr;
        }
        m_isChanged = true;
    }

private:

    MACRO_DISABLE_COPY_CONSTRUCTOR(DirectX11ConstantBuffer);

    T                                m_data;
    std::unique_ptr<DirectX11Buffer> m_upBuffer  = nullptr;
    bool                             m_isChanged = true;

};

#endif
