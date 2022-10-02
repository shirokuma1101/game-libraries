﻿#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11_H_

#include <memory>
#include <utility>

#include <windows.h>
#include <wrl/client.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include "Utility/Assert.h"
#include "DirectX11Helper.h"
#include "DirectX11Texture.h"

class DirectX11
{
public:

    virtual ~DirectX11() {
        Release();
        m_cpDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    }

    virtual bool Init(HWND hWnd, const std::pair<int32_t, int32_t>& size, bool is_debug, bool enable_msaa = false) {
        CreateFactory();
        CreateDevice(is_debug);
        CreateSwapChain(hWnd, size, enable_msaa);
        SetState();
        CreateBackBuffer();
        CreateDepthStencilView(size);
        CreateViewport(size);

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

    auto GetDev() {
        return m_cpDev;
    }
    auto GetDev() const {
        return m_cpDev;
    }
    auto GetCtx() {
        return m_cpCtx;
    }
    auto GetCtx() const {
        return m_cpCtx;
    }
    auto GetSwapChain() {
        return m_cpSwapChain;
    }
    auto GetSwapChain() const {
        return m_cpSwapChain;
    }
    auto GetBackBuffer() {
        return m_spBackBuffer;
    }
    auto GetBackBuffer() const {
        return m_spBackBuffer;
    }
    auto GetZBuffer() {
        return m_spZBuffer;
    }
    auto GetZBuffer() const {
        return m_spZBuffer;
    }
    auto GetWhiteTexture() {
        return m_spWhiteTexture;
    }
    auto GetNormalTexture() {
        return m_spNormalTexture;
    }

protected:

    void CreateFactory() {
        if (FAILED(CreateDXGIFactory(IID_PPV_ARGS(&m_cpFactory)))) {
            assert::RaiseAssert("Create factory failed");
        }

        UINT   limit          = 100; // 列挙上限
        UINT   index          = 0;   // インデックス
        UINT   gpu_number     = 0;   // GPU番号
        SIZE_T gpu_max_memory = 0;   // 最大VRAM

        while (true) {
            DXGI_ADAPTER_DESC ad{};
            SecureZeroMemory(&ad, sizeof(ad));

            /* GPUを列挙 */
            if (DXGI_ERROR_NOT_FOUND == m_cpFactory->EnumAdapters(index, &m_cpAdapter)) break;
            m_cpAdapter->GetDesc(&ad);

            /* 列挙したVRAMが大きかったら代入*/
            if (ad.DedicatedVideoMemory > gpu_max_memory) {
                gpu_max_memory = ad.DedicatedVideoMemory;
                gpu_number = index;
            }

            if (index >= limit) break;

            ++index;
        }

        /* 決定したGPUを設定する */
        if (FAILED(m_cpFactory->EnumAdapters(gpu_number, &m_cpAdapter))) {
            assert::RaiseAssert("Enumerate adapters failed");
        }
    }
    void CreateDevice(bool is_debug = false) {
        /* ドライバーの種類 */
        D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_HARDWARE;
        // アダプタが指定されていた場合はD3D_DRIVER_TYPE_UNKNOWNを指定する
        if (m_cpAdapter.Get()) {
            driver_type = D3D_DRIVER_TYPE_UNKNOWN;
        }

        /* ソフトウェアラスタライザを実装するDLLハンドル */
        HMODULE software{};
        if (driver_type == D3D_DRIVER_TYPE_SOFTWARE) {
            if (!software) assert::RaiseAssert("Software rasterizer not found");
        }

        /* ランタイムレイヤのフラグ */
        UINT flags = 0;
        // デバッグレイヤーを有効にする
        if (is_debug) {
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
            feature_levels,           // D3D_FEATURE_LEVEL指定で自分で定義した配列を指定可能
            _countof(feature_levels), // 上のD3D_FEATURE_LEVEL配列の要素数
            D3D11_SDK_VERSION,        // SDKバージョン
            &m_cpDev,                 // Deviceの出力先
            &feature_level,           // D3D_FEATURE_LEVELの出力先
            &m_cpCtx                  // DeviceContextの出力先
        ))) {
            assert::RaiseAssert("Create device failed");
        }

        if (is_debug) {
            /* デバッグインターフェースを取得 */
            if (FAILED(m_cpDev->QueryInterface(IID_PPV_ARGS(&m_cpDebug)))) {
                assert::RaiseAssert("Query debug interface failed");
            }
        }
    }
    void CreateSwapChain(HWND hWnd, const std::pair<int32_t, int32_t>& size, bool enable_msaa) {
        DXGI_SWAP_CHAIN_DESC sd{};
        SecureZeroMemory(&sd, sizeof(sd));
        directx11_helper::SetUpSwapChainDesc(&sd, size, m_cpDev.Get(), m_cpAdapter.Get(), hWnd, enable_msaa);
        if (FAILED(m_cpFactory->CreateSwapChain(m_cpDev.Get(), &sd, &m_cpSwapChain))) {
            assert::RaiseAssert("Create swap chain failed");
        }
    }
    bool CreateBackBuffer() {
        ID3D11Texture2D* back_buffer = nullptr;
        m_spBackBuffer = std::make_shared<DirectX11Texture>(m_cpDev.Get(), m_cpCtx.Get());
        if (FAILED(m_cpSwapChain->GetBuffer(0, IID_PPV_ARGS(&back_buffer)))) {
            assert::RaiseAssert("Get back buffer failed");
            return false;
        }
        if (!m_spBackBuffer->Create(back_buffer)) {
            assert::RaiseAssert("Create back buffer failed");
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
            assert::RaiseAssert("Create depth stencil view failed");
            return false;
        }
    
        ID3D11RenderTargetView* rtvs[] = {
            m_spBackBuffer->GetRtv()
        };
        m_cpCtx->OMSetRenderTargets(1, rtvs, m_spZBuffer->GetDsv());
        return true;
    }
    void CreateViewport(const std::pair<int32_t, int32_t>& size) {
        D3D11_VIEWPORT vp{};
        vp.TopLeftX = 0.f;
        vp.TopLeftY = 0.f;
        vp.Width    = static_cast<FLOAT>(size.first);
        vp.Height   = static_cast<FLOAT>(size.second);
        vp.MinDepth = 0.f;
        vp.MaxDepth = 1.f;
        m_cpCtx->RSSetViewports(1, &vp);
    }
    void SetState() {
        /* ブレンドステート */
        ID3D11BlendState* bs = directx11_helper::CreateBlendState(m_cpDev.Get(), directx11_helper::BlendMode::Alpha);
        FLOAT blend_factor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        m_cpCtx->OMSetBlendState(bs, blend_factor, 0xFFFFFFFF);
        memory::SafeRelease(&bs);

        /* サンプラーステートセット */
        ID3D11SamplerState* ss0 = directx11_helper::CreateSamplerState(
            m_cpDev.Get(), directx11_helper::SamplerFilterMode::Anisotropic, 4, directx11_helper::SamplerAddressMode::Wrap, false
        );
        ID3D11SamplerState* ss1 = directx11_helper::CreateSamplerState(
            m_cpDev.Get(), directx11_helper::SamplerFilterMode::Anisotropic, 4, directx11_helper::SamplerAddressMode::Clamp, false
        );
        m_cpCtx->VSSetSamplers(0, 1, &ss0);
        m_cpCtx->PSSetSamplers(0, 1, &ss0);
        m_cpCtx->GSSetSamplers(0, 1, &ss0);
        m_cpCtx->CSSetSamplers(0, 1, &ss0);
        m_cpCtx->VSSetSamplers(1, 1, &ss1);
        m_cpCtx->PSSetSamplers(1, 1, &ss1);
        m_cpCtx->GSSetSamplers(1, 1, &ss1);
        m_cpCtx->CSSetSamplers(1, 1, &ss1);
        memory::SafeRelease(&ss0);
        memory::SafeRelease(&ss1);

        /* ラスタライザーステート */
        ID3D11RasterizerState* rs = directx11_helper::CreateRasterizerState(
            m_cpDev.Get(), D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK, true, false
        );
        m_cpCtx->RSSetState(rs);
        memory::SafeRelease(&rs);

        /* 深度ステンシルステート */
        ID3D11DepthStencilState* dss = directx11_helper::CreateDepthStencilState(m_cpDev.Get(), true, true);
        m_cpCtx->OMSetDepthStencilState(dss, 0);
        memory::SafeRelease(&dss);
    }

    void Release() {
        m_spZBuffer.reset();
        m_spBackBuffer.reset();
        
        m_cpFactory.Reset();
        m_cpAdapter.Reset();
        m_cpSwapChain.Reset();

        m_cpCtx.Reset();
        m_cpDev.Reset();
    }

    ComPtr<ID3D11Device>        m_cpDev       = nullptr;
    ComPtr<ID3D11DeviceContext> m_cpCtx       = nullptr;
    
    ComPtr<IDXGISwapChain>      m_cpSwapChain = nullptr;
    ComPtr<IDXGIAdapter>        m_cpAdapter   = nullptr;
    ComPtr<IDXGIFactory>        m_cpFactory   = nullptr;

    ComPtr<ID3D11Debug>         m_cpDebug     = nullptr;
    
    std::shared_ptr<DirectX11Texture> m_spBackBuffer = nullptr;
    std::shared_ptr<DirectX11Texture> m_spZBuffer    = nullptr;
    
    std::shared_ptr<DirectX11Texture> m_spWhiteTexture  = nullptr;
    std::shared_ptr<DirectX11Texture> m_spNormalTexture = nullptr;

};

#endif