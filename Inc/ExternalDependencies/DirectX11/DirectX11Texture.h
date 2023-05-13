#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11TEXTURE_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11TEXTURE_H_

#include <filesystem>
#include <string>
#include <string_view>
#include <utility>
#include <d3d11_4.h>

#include "Utility/Assert.h"
#include "Utility/Macro.h"
#include "Utility/Memory.h"
#include "DirectX11Helper.h"

#include "DirectXTex.h"
#pragma comment(lib, "DirectXTex.lib")
#include "strconv.h"

class DirectX11Texture
{
public:

    DirectX11Texture(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_pDev(dev)
        , m_pCtx(ctx)
    {}
    virtual ~DirectX11Texture() {
        Release();
    }
    
    MACRO_GETTER_PTR(ID3D11ShaderResourceView, GetSrv, m_pSrv);
    MACRO_GETTER_PTR(ID3D11RenderTargetView,   GetRtv, m_pRtv);
    MACRO_GETTER_PTR(ID3D11DepthStencilView,   GetDsv, m_pDsv);
    MACRO_GETTER_PTR_ADDR(ID3D11ShaderResourceView, GetSrvAddress, &m_pSrv);
    MACRO_GETTER_PTR_ADDR(ID3D11RenderTargetView,   GetRtvAddress, &m_pRtv);
    MACRO_GETTER_PTR_ADDR(ID3D11DepthStencilView,   GetDsvAddress, &m_pDsv);

    const std::string& GetFilePath() const noexcept {
        return m_filePath;
    }
    const D3D11_TEXTURE2D_DESC& GetTextureDesc() const noexcept {
        return m_texture2dDesc;
    }
    ID3D11Texture2D* GetTexture() const {
        if (!m_pSrv) {
            return nullptr;
        }

        ID3D11Resource* resource = nullptr;
        m_pSrv->GetResource(&resource);

        ID3D11Texture2D* texture2d = nullptr;
        if (FAILED(resource->QueryInterface<ID3D11Texture2D>(&texture2d))) {
            memory::SafeRelease(&resource);
            return nullptr;
        }
        memory::SafeRelease(&resource);
        texture2d->Release();

        return texture2d;
    }

    bool Load(std::string_view file_path, bool render_target = false, bool depth_stencil = false, bool generate_mipmap = true) {
        m_filePath = file_path;
        DirectX::TexMetadata metadata{};
        DirectX::ScratchImage image{};
        std::string ext = std::filesystem::path(m_filePath).extension().string();

        UINT bind_flags = D3D11_BIND_SHADER_RESOURCE;
        if (render_target) bind_flags |= D3D11_BIND_RENDER_TARGET;
        if (depth_stencil) bind_flags |= D3D11_BIND_DEPTH_STENCIL;

        // .dds
        if (ext == ".dds") {
            if (FAILED(LoadFromDDSFile(sjis_to_wide(m_filePath).data(), DirectX::DDS_FLAGS_NONE, &metadata, image))) {
                return false;
            }
        }
        // .tga
        else if (ext == ".tga") {
            if (FAILED(LoadFromTGAFile(sjis_to_wide(m_filePath).data(), &metadata, image))) {
                return false;
            }
        }
        // .bmp .png .gif .tiff ,jpeg
        else {
            if (FAILED(LoadFromWICFile(sjis_to_wide(m_filePath).data(), DirectX::WIC_FLAGS::WIC_FLAGS_ALL_FRAMES, &metadata, image))) {
                return false;
            }
        }

        if (generate_mipmap && metadata.mipLevels == 1) {
            DirectX::ScratchImage mip_chain;
            if (SUCCEEDED(DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT, 0, mip_chain))) {
                image.Release();
                image = std::move(mip_chain);
            }
        }

        ID3D11Texture2D* texture2d = nullptr;
        if (FAILED(DirectX::CreateTextureEx(
            m_pDev,
            image.GetImages(),
            image.GetImageCount(),
            image.GetMetadata(),
            D3D11_USAGE_DEFAULT,
            bind_flags,
            0,
            0,
            false,
            (ID3D11Resource**)&texture2d
        ))) {
            return false;
        }

        if (!directx11_helper::CreateTexture2D(m_pDev, texture2d, &m_pSrv, &m_pRtv, &m_pDsv)) {
            memory::SafeRelease(&texture2d);
            return false;
        }

        texture2d->GetDesc(&m_texture2dDesc);
        memory::SafeRelease(&texture2d);

        return true;
    }

    bool Create(ID3D11Texture2D* texture2d) {
        Release();

        if (!texture2d) {
            return false;
        }

        if (!directx11_helper::CreateTexture2D(m_pDev, texture2d, &m_pSrv, &m_pRtv, &m_pDsv)) {
            Release();
            return false;
        }

        texture2d->GetDesc(&m_texture2dDesc);

        return true;

    }
    bool Create(const D3D11_TEXTURE2D_DESC& td, const D3D11_SUBRESOURCE_DATA* sd = nullptr) {
        Release();

        ID3D11Texture2D* resource;
        if (FAILED(m_pDev->CreateTexture2D(&td, sd, &resource))) {
            Release();
            return false;
        }

        if (!directx11_helper::CreateTexture2D(m_pDev, resource, &m_pSrv, &m_pRtv, &m_pDsv)) {
            Release();
            return false;
        }

        // 画像情報取得
        resource->GetDesc(&m_texture2dDesc);
        memory::SafeRelease(&resource);

        return true;
    }
    bool Create(const std::pair<int32_t, int32_t>& size, UINT array_size, DXGI_FORMAT format, const D3D11_SUBRESOURCE_DATA* sd = nullptr) {
        Release();

        D3D11_TEXTURE2D_DESC td{};
        td.Width              = static_cast<UINT>(size.first);
        td.Height             = static_cast<UINT>(size.second);
        td.MiscFlags          = 0;
        td.ArraySize          = array_size;
        td.Format             = format;
        td.SampleDesc.Count   = 1;
        td.SampleDesc.Quality = 0;
        td.Usage              = D3D11_USAGE_DEFAULT;
        td.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags     = 0;
        td.MipLevels          = 1;

        if (!Create(td, sd)) {
            return false;
        }

        return true;
    }

    bool CreateRenderTarget(const std::pair<int32_t, int32_t>& size, UINT array_size = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, const D3D11_SUBRESOURCE_DATA* sd = nullptr) {
        Release();

        D3D11_TEXTURE2D_DESC td{};
        td.Width              = static_cast<UINT>(size.first);
        td.Height             = static_cast<UINT>(size.second);
        td.MiscFlags          = 0;
        td.ArraySize          = array_size;
        td.Format             = format;
        td.SampleDesc.Count   = 1;
        td.SampleDesc.Quality = 0;
        td.Usage              = D3D11_USAGE_DEFAULT;
        td.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        td.CPUAccessFlags     = 0;
        td.MipLevels          = 1;

        if (!Create(td, sd)) {
            return false;
        }
        return true;
    }
    bool CreateDepthStencil(const std::pair<int32_t, int32_t>& size, UINT array_size = 1, DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS, const D3D11_SUBRESOURCE_DATA* sd = nullptr) {
        Release();

        D3D11_TEXTURE2D_DESC td{};
        td.Width              = static_cast<UINT>(size.first);
        td.Height             = static_cast<UINT>(size.second);
        td.MiscFlags          = 0;
        td.ArraySize          = array_size;
        td.Format             = format;
        td.SampleDesc.Count   = 1;
        td.SampleDesc.Quality = 0;
        td.Usage              = D3D11_USAGE_DEFAULT;
        td.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
        td.CPUAccessFlags     = 0;
        td.MipLevels          = 1;

        if (!Create(td, sd)) {
            return false;
        }
        return true;
    }

    static void ConvertRectToUV(const DirectX11Texture& texture, const DirectX::SimpleMath::Rectangle& rect, DirectX::SimpleMath::Vector2* uv_min, DirectX::SimpleMath::Vector2* uv_max) {
        uv_min->x = rect.x / static_cast<float>(texture.GetTextureDesc().Width);
        uv_min->y = rect.y / static_cast<float>(texture.GetTextureDesc().Height);
        uv_max->x = (rect.width / static_cast<float>(texture.GetTextureDesc().Width)) + uv_min->x;
        uv_max->y = (rect.height / static_cast<float>(texture.GetTextureDesc().Height)) + uv_min->y;
    }

private:

    void Release() {
        memory::SafeRelease(&m_pSrv);
        memory::SafeRelease(&m_pRtv);
        memory::SafeRelease(&m_pDsv);
    }

    ID3D11Device*             m_pDev = nullptr;
    ID3D11DeviceContext*      m_pCtx = nullptr;

    ID3D11ShaderResourceView* m_pSrv = nullptr;
    ID3D11RenderTargetView*   m_pRtv = nullptr;
    ID3D11DepthStencilView*   m_pDsv = nullptr;
    std::string               m_filePath;
    D3D11_TEXTURE2D_DESC      m_texture2dDesc{};

};

class DirectX11RenderTarget
{
public:

    DirectX11RenderTarget(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_pDev(dev)
        , m_pCtx(ctx)
    {
        if (!m_pDev || !m_pCtx) {
            assert::ShowError(ASSERT_FILE_LINE, "device or device context is nullptr");
        }
    }
    virtual ~DirectX11RenderTarget() noexcept {
        Release();
    }
    
    MACRO_GETTER_SHARED_PTR(DirectX11Texture, GetBackBuffer, m_spBackBuffer);
    MACRO_GETTER_SHARED_PTR(DirectX11Texture, GetZBuffer,    m_spZBuffer);
    MACRO_GETTER_CONST_REF(D3D11_VIEWPORT, GetViewport, m_viewport);

    void GetFromCurrent() {
        Release();

        m_spBackBuffer = std::make_shared<DirectX11Texture>(m_pDev, m_pCtx);
        m_spZBuffer = std::make_shared<DirectX11Texture>(m_pDev, m_pCtx);
        m_pCtx->OMGetRenderTargets(1, m_spBackBuffer->GetRtvAddress(), m_spZBuffer->GetDsvAddress());
        D3D11_VIEWPORT v{};
        UINT num = 1;
        m_pCtx->RSGetViewports(&num, &v);
        m_viewport = v;
    }

    void Set() {
        if (m_spBackBuffer) {
            m_pCtx->OMSetRenderTargets(1, m_spBackBuffer->GetRtvAddress(), m_spZBuffer ? m_spZBuffer->GetDsv() : nullptr);
        }
        m_pCtx->RSSetViewports(1, &m_viewport);
    }

    void Create(const std::pair<int32_t, int32_t>& size, bool zbuffer, DXGI_FORMAT rt_format, DXGI_FORMAT ds_format, const D3D11_VIEWPORT& v) {
        Release();

        m_spBackBuffer = std::make_shared<DirectX11Texture>(m_pDev, m_pCtx);
        m_spBackBuffer->CreateRenderTarget(size, 1, rt_format);
        if (zbuffer) {
            m_spZBuffer = std::make_shared<DirectX11Texture>(m_pDev, m_pCtx);
            m_spZBuffer->CreateDepthStencil(size, 1, ds_format);
        }
        m_viewport = v;
    }
    void Create(const std::pair<int32_t, int32_t>& size, bool zbuffer = false, DXGI_FORMAT rt_format = DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT ds_format = DXGI_FORMAT_R24G8_TYPELESS) {
        D3D11_VIEWPORT v{};
        v.TopLeftX = 0.f;
        v.TopLeftY = 0.f;
        v.Width    = static_cast<float>(size.first);
        v.Height   = static_cast<float>(size.second);
        v.MinDepth = 0.f;
        v.MaxDepth = 1.f;

        Create(size, zbuffer, rt_format, ds_format, v);
    }

    void Clear(const FLOAT color[4] = directx11_helper::alpha) {
        if (m_spBackBuffer) {
            m_pCtx->ClearRenderTargetView(m_spBackBuffer->GetRtv(), color);
        }
        if (m_spZBuffer) {
            m_pCtx->ClearDepthStencilView(m_spZBuffer->GetDsv(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        }
    }

private:

    void Release() noexcept {
        m_spBackBuffer.reset();
        m_spBackBuffer = nullptr;
        m_spZBuffer.reset();
        m_spZBuffer = nullptr;
    }

    ID3D11Device*                     m_pDev         = nullptr;
    ID3D11DeviceContext*              m_pCtx         = nullptr;

    std::shared_ptr<DirectX11Texture> m_spBackBuffer = nullptr;
    std::shared_ptr<DirectX11Texture> m_spZBuffer    = nullptr;
    D3D11_VIEWPORT                    m_viewport     = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

};

class DirectX11RenderTargetChanger
{
public:

    DirectX11RenderTargetChanger(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_pDev(dev)
        , m_pCtx(ctx)
    {
        if (!m_pDev || !m_pCtx) {
            assert::ShowError(ASSERT_FILE_LINE, "device or device context is nullptr");
        }
    }
    ~DirectX11RenderTargetChanger() noexcept {
        Release();
    }

    void Change(DirectX11RenderTarget* rt) {
        if (!m_spSaveRT) {
            m_spSaveRT = std::make_shared<DirectX11RenderTarget>(m_pDev, m_pCtx);
            m_spSaveRT->GetFromCurrent();
        }
        if (rt) {
            rt->Set();
        }
    }

    void Undo(bool release = false) {
        if (m_spSaveRT) {
            m_spSaveRT->Set();
            if (release) {
                Release();
            }
        }
    }

private:

    void Release() noexcept {
        m_spSaveRT.reset();
        m_spSaveRT = nullptr;
    }

    ID3D11Device*                          m_pDev = nullptr;
    ID3D11DeviceContext*                   m_pCtx = nullptr;

    std::shared_ptr<DirectX11RenderTarget> m_spSaveRT = nullptr;

};

#endif
