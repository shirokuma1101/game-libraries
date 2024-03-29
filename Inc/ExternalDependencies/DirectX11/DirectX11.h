﻿#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11_H_

#include <memory>
#include <utility>

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include "Utility/Assert.h"
#include "Utility/Macro.h"
#include "DirectX11Helper.h"
#include "DirectX11Texture.h"

class DirectX11
{
public:

    virtual ~DirectX11() {
        Release();
    }

    MACRO_GETTER_AS_IS(ComPtr<ID3D11Device>,        GetDev,       m_cpDev);
    MACRO_GETTER_AS_IS(ComPtr<ID3D11DeviceContext>, GetCtx,       m_cpCtx);
    MACRO_GETTER_AS_IS(ComPtr<IDXGISwapChain>,      GetSwapChain, m_cpSwapChain);

    MACRO_GETTER_SHARED_PTR(DirectX11Texture, GetBackBuffer,    m_spBackBuffer);
    MACRO_GETTER_SHARED_PTR(DirectX11Texture, GetZBuffer,       m_spZBuffer);
    MACRO_GETTER_SHARED_PTR(DirectX11Texture, GetWhiteTexture,  m_spWhiteTexture);
    MACRO_GETTER_SHARED_PTR(DirectX11Texture, GetNormalTexture, m_spNormalTexture);

    D3D11_VIEWPORT GetViewport(UINT num = 1) const {
        D3D11_VIEWPORT v{};
        m_cpCtx->RSGetViewports(&num, &v);
        return v;
    }

    virtual bool Init(HWND hWnd, const std::pair<int32_t, int32_t>& size, bool enable_debug, bool enable_detailed_memory_infomation, bool enable_msaa = false) {
        CreateFactory();
        CreateDevice(enable_debug, enable_detailed_memory_infomation);
        CreateSwapChain(hWnd, size, enable_msaa);
        CreateBackBuffer();
        CreateDepthStencilView(size);
        CreateViewport(size);

        // Create 1x1 white texture
        {
            D3D11_SUBRESOURCE_DATA sd{};
            SecureZeroMemory(&sd, sizeof(sd));
            auto color = directx11_helper::white.RGBA();
            sd.pSysMem          = &color;
            sd.SysMemPitch      = 4;
            sd.SysMemSlicePitch = 0;
            m_spWhiteTexture = std::make_shared<DirectX11Texture>(m_cpDev.Get(), m_cpCtx.Get());
            m_spWhiteTexture->Create({ 1, 1 }, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &sd);
        }
        // Create 1x1 normal texture
        {
            D3D11_SUBRESOURCE_DATA sd{};
            SecureZeroMemory(&sd, sizeof(sd));
            auto color = directx11_helper::normal.RGBA();
            sd.pSysMem          = &color;
            sd.SysMemPitch      = 4;
            sd.SysMemSlicePitch = 0;
            m_spNormalTexture = std::make_shared<DirectX11Texture>(m_cpDev.Get(), m_cpCtx.Get());
            m_spNormalTexture->Create({ 1, 1 }, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &sd);
        }

        return true;
    }

    bool Resize(const std::pair<int32_t, int32_t>& size) {
        DXGI_SWAP_CHAIN_DESC scd{};
        SecureZeroMemory(&scd, sizeof(scd));
        m_cpSwapChain->GetDesc(&scd);
        
        ResetBuffers();
        
        if (FAILED(m_cpSwapChain->ResizeBuffers(scd.BufferCount, size.first, size.second, scd.BufferDesc.Format, scd.Flags))) {
            return false;
        }

        CreateBackBuffer();
        CreateDepthStencilView(size);
        CreateViewport(size);

        return true;
    }

    void ResetBuffers() {
        m_spBackBuffer.reset();
        m_spZBuffer.reset();
        m_cpCtx->OMSetRenderTargets(0, nullptr, nullptr);
    }

protected:
    
    ComPtr<ID3D11Device>        m_cpDev = nullptr;
    ComPtr<ID3D11DeviceContext> m_cpCtx = nullptr;

    ComPtr<IDXGISwapChain> m_cpSwapChain = nullptr;
    ComPtr<IDXGIAdapter>   m_cpAdapter   = nullptr;
    ComPtr<IDXGIFactory>   m_cpFactory   = nullptr;

    std::shared_ptr<DirectX11Texture> m_spBackBuffer = nullptr;
    std::shared_ptr<DirectX11Texture> m_spZBuffer    = nullptr;

    std::shared_ptr<DirectX11Texture> m_spWhiteTexture  = nullptr;
    std::shared_ptr<DirectX11Texture> m_spNormalTexture = nullptr;

private:

    void CreateFactory() {
        if (FAILED(CreateDXGIFactory(IID_PPV_ARGS(m_cpFactory.GetAddressOf())))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create factory failed.");
        }

        UINT   gpu_number     = 0; // GPU番号
        SIZE_T gpu_max_memory = 0; // 最大VRAM

        for (UINT i = 0; m_cpFactory->EnumAdapters(i, m_cpAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i) {
            DXGI_ADAPTER_DESC ad{};
            SecureZeroMemory(&ad, sizeof(ad));
            m_cpAdapter->GetDesc(&ad);

            if (ad.DedicatedVideoMemory > gpu_max_memory) {
                gpu_max_memory = ad.DedicatedVideoMemory;
                gpu_number = i;
            }
        }

        /* 決定したGPUを設定する */
        if (FAILED(m_cpFactory->EnumAdapters(gpu_number, m_cpAdapter.GetAddressOf()))) {
            assert::ShowError(ASSERT_FILE_LINE, "Enumerate adapters failed.");
        }
    }
    void CreateDevice(bool enable_debug = false, bool enable_detailed_memory_infomation = false) {
        /* ドライバーの種類 */
        D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_HARDWARE;
        // アダプタが指定されていた場合はD3D_DRIVER_TYPE_UNKNOWNを指定する
        if (m_cpAdapter) {
            driver_type = D3D_DRIVER_TYPE_UNKNOWN;
        }

        /* ソフトウェアラスタライザを実装するDLLハンドル */
        HMODULE software{};
        if (driver_type == D3D_DRIVER_TYPE_SOFTWARE) {
            if (!software) assert::ShowError(ASSERT_FILE_LINE, "Software rasterizer not found.");
        }

        /* ランタイムレイヤのフラグ */
        UINT flags = 0;
        // デバッグレイヤーを有効にする
        if (enable_debug) {
            flags |= D3D11_CREATE_DEVICE_DEBUG;
        }

        /* 機能レベルの指定 (降順) */
        D3D_FEATURE_LEVEL feature_levels[] = {
            //D3D_FEATURE_LEVEL_12_2, // Direct3D 12.2
            //D3D_FEATURE_LEVEL_12_1, // Direct3D 12.1
            //D3D_FEATURE_LEVEL_12_0, // Direct3D 12.0
            D3D_FEATURE_LEVEL_11_1, // Direct3D 11.1  ShaderModel 5
            D3D_FEATURE_LEVEL_11_0, // Direct3D 11    ShaderModel 5
            D3D_FEATURE_LEVEL_10_1, // Direct3D 10.1  ShaderModel 4
            D3D_FEATURE_LEVEL_10_0, // Direct3D 10.0  ShaderModel 4
            D3D_FEATURE_LEVEL_9_3,  // Direct3D 9.3   ShaderModel 3
            D3D_FEATURE_LEVEL_9_2,  // Direct3D 9.2   ShaderModel 3
            D3D_FEATURE_LEVEL_9_1,  // Direct3D 9.1   ShaderModel 3
        };
        /* 機能レベルの出力先 */
        D3D_FEATURE_LEVEL feature_level{};

        /* Device生成 */
        if (FAILED(D3D11CreateDevice(
            m_cpAdapter.Get(),        // 使用するディスプレイアダプタの指定
            driver_type,              // 描画を行うドライバの指定
            software,                 // D3D_DRIVER_TYPE_SOFTWARE指定時に使用
            flags,                    // デバッグ出力やシングルスレッドなどのランタイムレイヤのフラグ指定
            feature_levels,           // D3D_FEATURE_LEVEL指定で自分で定義した配列を指定
            _countof(feature_levels), // 上のD3D_FEATURE_LEVEL配列の要素数
            D3D11_SDK_VERSION,        // SDKバージョン
            m_cpDev.GetAddressOf(),   // Deviceの出力先
            &feature_level,           // D3D_FEATURE_LEVELの出力先
            m_cpCtx.GetAddressOf()    // DeviceContextの出力先
        ))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create device failed.");
        }

        if (enable_detailed_memory_infomation) {
            /* デバッグインターフェースを取得 */
            if (FAILED(m_cpDev->QueryInterface(IID_PPV_ARGS(m_cpDebug.GetAddressOf())))) {
                assert::ShowError(ASSERT_FILE_LINE, "Query debug interface failed.");
            }
        }
    }
    void CreateSwapChain(HWND hWnd, const std::pair<int32_t, int32_t>& size, bool enable_msaa) {
        DXGI_SWAP_CHAIN_DESC scd{};
        SecureZeroMemory(&scd, sizeof(scd));
        directx11_helper::SetUpSwapChainDesc(&scd, size, m_cpDev.Get(), m_cpAdapter.Get(), hWnd, enable_msaa);
        if (FAILED(m_cpFactory->CreateSwapChain(m_cpDev.Get(), &scd, m_cpSwapChain.GetAddressOf()))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create swap chain failed.");
        }
    }
    bool CreateBackBuffer() {
        ID3D11Texture2D* back_buffer = nullptr;
        m_spBackBuffer = std::make_shared<DirectX11Texture>(m_cpDev.Get(), m_cpCtx.Get());
        if (FAILED(m_cpSwapChain->GetBuffer(0, IID_PPV_ARGS(&back_buffer)))) {
            assert::ShowError(ASSERT_FILE_LINE, "Get back buffer failed.");
            return false;
        }
        if (!m_spBackBuffer->Create(back_buffer)) {
            assert::ShowError(ASSERT_FILE_LINE, "Create back buffer failed.");
            return false;
        }
        memory::SafeRelease(&back_buffer);
        return true;
    }
    bool CreateDepthStencilView(const std::pair<int32_t, int32_t>& size) {
        D3D11_TEXTURE2D_DESC td{};
        td.Width              = static_cast<UINT>(size.first);
        td.Height             = static_cast<UINT>(size.second);
        td.MipLevels          = 1;
        td.ArraySize          = 1;
        td.Format             = DXGI_FORMAT_R24G8_TYPELESS;
        td.SampleDesc.Count   = 1;
        td.SampleDesc.Quality = 0;
        td.Usage              = D3D11_USAGE_DEFAULT;
        td.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

        m_spZBuffer = std::make_shared<DirectX11Texture>(m_cpDev.Get(), m_cpCtx.Get());
        
        if (!m_spZBuffer->Create(td)) {
            assert::ShowError(ASSERT_FILE_LINE, "Create depth stencil view failed.");
            return false;
        }
    
        ID3D11RenderTargetView* rtvs[] = {
            m_spBackBuffer->GetRtv()
        };
        m_cpCtx->OMSetRenderTargets(1, rtvs, m_spZBuffer->GetDsv());
        return true;
    }
    void CreateViewport(const std::pair<int32_t, int32_t>& size) {
        D3D11_VIEWPORT v{};
        v.TopLeftX = 0.f;
        v.TopLeftY = 0.f;
        v.Width    = static_cast<FLOAT>(size.first);
        v.Height   = static_cast<FLOAT>(size.second);
        v.MinDepth = 0.f;
        v.MaxDepth = 1.f;
        m_cpCtx->RSSetViewports(1, &v);
    }

    void Release() {
        ResetBuffers();

        m_cpFactory.Reset();
        m_cpAdapter.Reset();
        m_cpSwapChain.Reset();

        m_cpCtx.Reset();
        m_cpDev.Reset();

        if (m_cpDebug) {
            m_cpDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        }
        m_cpDebug.Reset();
    }

    ComPtr<ID3D11Debug> m_cpDebug = nullptr;

};

#endif
