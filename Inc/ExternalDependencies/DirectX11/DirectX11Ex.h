#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11_H_

#include <memory>
#include <utility>

#include <Windows.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include "Utility/Assert.h"
#include "Utility/Macro.h"
#include "DirectX11Helper.h"
#include "DirectX11Texture.h"

class DirectX11Ex
{
public:

    virtual ~DirectX11Ex() {
        Release();
    }

    MACRO_GETTER_AS_IS(ComPtr<ID3D11Device>,        GetDev,       m_cpDev);
    MACRO_GETTER_AS_IS(ComPtr<ID3D11DeviceContext>, GetCtx,       m_cpCtx);
    MACRO_GETTER_AS_IS(ComPtr<IDXGISwapChain>,      GetSwapChain, m_cpSwapChain);

    D3D11_VIEWPORT GetViewport(UINT num = 1) const {
        D3D11_VIEWPORT v{};
        m_cpCtx->RSGetViewports(&num, &v);
        return v;
    }

    bool Init(HWND hWnd, const std::pair<int32_t, int32_t>& size, bool enable_debug, bool enable_detailed_memory_infomation, bool enable_msaa = false) {
        CreateFactory();
        CreateDevice(enable_debug, enable_detailed_memory_infomation);
        CreateSwapChain(hWnd, size, enable_msaa);
        //CreateBackBuffer();
        //CreateDepthStencilView(size);
        CreateViewport(size);

        return true;
    }

private:

    void CreateFactory() {

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

        /* デバイスとデバイスコンテキストの作成 */
        ComPtr<ID3D11Device> dev;
        ComPtr<ID3D11DeviceContext> ctx;
        
        if (FAILED(D3D11CreateDevice(
            m_cpAdapter.Get(),
            driver_type,
            software,
            flags,
            feature_levels,
            _countof(feature_levels),
            D3D11_SDK_VERSION,
            dev.GetAddressOf(),
            &feature_level,
            ctx.GetAddressOf()
        ))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create device failed.");
        }
        
        dev.As(&m_cpDev);
        ctx.As(&m_cpCtx);

        // First, convert our ID3D11Device1 into an IDXGIDevice1
        ComPtr<IDXGIDevice4> dxgi_dev;
        m_cpDev.As(&dxgi_dev);

        // Second, use the IDXGIDevice1 interface to get access to the adapter
        ComPtr<IDXGIAdapter> dxgi_adapter;
        dxgi_dev->GetAdapter(&dxgi_adapter);

        // Third, use the IDXGIAdapter interface to get access to the factory
        dxgi_adapter->GetParent(IID_PPV_ARGS(&m_cpFactory));
        
        if (enable_detailed_memory_infomation) {
            /* デバッグインターフェースを取得 */
            if (FAILED(m_cpDev->QueryInterface(IID_PPV_ARGS(m_cpDebug.GetAddressOf())))) {
                assert::ShowError(ASSERT_FILE_LINE, "Query debug interface failed.");
            }
        }
    }
    void CreateSwapChain(HWND hWnd, const std::pair<int32_t, int32_t>& size, bool enable_msaa) {
        DXGI_SWAP_CHAIN_DESC1 scd1{};
        SecureZeroMemory(&scd1, sizeof(scd1));

        //CoreWindow^ Window = CoreWindow::GetForCurrentThread();    // get the window pointer
        
        //m_cpFactory->CreateSwapChainForCoreWindow(
        //    dev.Get(),
        //    reinterpret_cast<IUnknown*>(Window),
        //    &scd,
        //    nullptr,
        //    &m_cpSwapChain);

    }
    void CreateViewport(const std::pair<int32_t, int32_t>& size) {
        D3D11_VIEWPORT v{};
        v.TopLeftX = 0.f;
        v.TopLeftY = 0.f;
        v.Width = static_cast<FLOAT>(size.first);
        v.Height = static_cast<FLOAT>(size.second);
        v.MinDepth = 0.f;
        v.MaxDepth = 1.f;
        m_cpCtx->RSSetViewports(1, &v);
    }

    void Release() {
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

    ComPtr<ID3D11Device5>        m_cpDev = nullptr;
    ComPtr<ID3D11DeviceContext4> m_cpCtx = nullptr;

    ComPtr<IDXGISwapChain4> m_cpSwapChain = nullptr;
    ComPtr<IDXGIAdapter4>   m_cpAdapter = nullptr;
    ComPtr<IDXGIFactory7>   m_cpFactory = nullptr;

    ComPtr<ID3D11Debug> m_cpDebug = nullptr;
    
};

#endif
