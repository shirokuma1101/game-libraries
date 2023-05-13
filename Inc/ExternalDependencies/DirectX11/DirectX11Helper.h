#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11HELPER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11HELPER_H_

#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <utility>
#include <wrl/client.h>
#pragma comment (lib, "D3DCompiler.lib")

#include "Utility/Assert.h"
#include "Utility/Memory.h"

#include "SimpleMath.h"
#include "strconv.h"

using Microsoft::WRL::ComPtr;


namespace directx11_helper {

    /**************************************************
    * Constant helper
    **************************************************/

    constexpr DirectX::SimpleMath::Color white   = { 1.0f, 1.0f, 1.0f, 1.0f };
    constexpr DirectX::SimpleMath::Color black   = { 0.0f, 0.0f, 0.0f, 1.0f };
    constexpr DirectX::SimpleMath::Color alpha   = { 0.0f, 0.0f, 0.0f, 0.0f };

    constexpr DirectX::SimpleMath::Color red     = { 1.0f, 0.0f, 0.0f, 1.0f };
    constexpr DirectX::SimpleMath::Color green   = { 0.0f, 1.0f, 0.0f, 1.0f };
    constexpr DirectX::SimpleMath::Color blue    = { 0.0f, 0.0f, 1.0f, 1.0f };
    constexpr DirectX::SimpleMath::Color yellow  = { 1.0f, 1.0f, 0.0f, 1.0f };
    constexpr DirectX::SimpleMath::Color cyan    = { 0.0f, 1.0f, 1.0f, 1.0f };
    constexpr DirectX::SimpleMath::Color magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

    constexpr DirectX::SimpleMath::Color normal  = { 0.5f, 0.5f, 1.0f, 1.0f };

    #define DIRECTX11_HELPER_PADDING_1BYTE(num)  const char padding1byte##num[1]   = { 0 }
    #define DIRECTX11_HELPER_PADDING_2BYTE(num)  const char padding2byte##num[2]   = { 0, 0 }
    #define DIRECTX11_HELPER_PADDING_3BYTE(num)  const char padding3byte##num[3]   = { 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_4BYTE(num)  const char padding4byte##num[4]   = { 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_5BYTE(num)  const char padding5byte##num[5]   = { 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_6BYTE(num)  const char padding6byte##num[6]   = { 0, 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_7BYTE(num)  const char padding7byte##num[7]   = { 0, 0, 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_8BYTE(num)  const char padding8byte##num[8]   = { 0, 0, 0, 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_9BYTE(num)  const char padding9byte##num[9]   = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_10BYTE(num) const char padding10byte##num[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_11BYTE(num) const char padding11byte##num[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_12BYTE(num) const char padding12byte##num[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_13BYTE(num) const char padding13byte##num[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_14BYTE(num) const char padding14byte##num[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_15BYTE(num) const char padding15byte##num[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    #define DIRECTX11_HELPER_PADDING_16BYTE(num) const char padding16byte##num[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }


    /**************************************************
    * enum class
    **************************************************/

    /* ShaderTarget */

    enum class ShaderTarget {
        CS, // Compute Shader
        DS, // Domain Shader
        GS, // Geometry Shader
        HS, // Hull Shader
        PS, // Pixel Shader
        VS, // Vertex Shader
    };

    /* DepthStencilState */

    //enum class StencilOp {
    //    KEEP,              // ステンシル値を保持
    //    ZERO,              // ステンシル値を0に設定
    //    REPLACE,           // ステンシル値を参照値で置き換える
    //    INCR_SAT,          // ステンシル値をインクリメントし、最大値を超えた場合は最大値に設定
    //    DECR_SAT,          // ステンシル値をデクリメントし、最小値を下回った場合は最小値に設定
    //    INVERT,            // ステンシル値をビット反転
    //    INCR,              // ステンシル値をインクリメント
    //    DECR,              // ステンシル値をデクリメント
    //};

    /* RasterizerState */

    enum class FillMode {
        SOLID,             // ソリッド
        WIREFRAME,         // ワイヤーフレーム
    };
    enum class CullMode {
        NONE,              // カリングなし
        FRONT,             // 前面をカリング
        BACK,              // 後面をカリング
    };

    /* SamplerState */

    enum class SamplerFilterMode {
        POINT,             // ポイントサンプリング
        LINEAR,            // バイリニアサンプリング
        ANISOTROPIC,       // 異方性フィルタリング
    };
    enum class SamplerAddressMode {
        WRAP,              // テクスチャの端を繰り返す
        MIRROR,            // テクスチャの端を反転して繰り返す
        CLAMP,             // テクスチャの端をクランプする
        BORDER,            // テクスチャの端をボーダーカラーでクランプする
        MIRROR_ONCE,       // テクスチャの端を反転して繰り返す
    };
    //enum class SamplerComparisonFunc {
    //    NEVER,             // 比較関数なし
    //    LESS,              // 比較関数：<
    //    EQUAL,             // 比較関数：==
    //    LESS_EQUAL,        // 比較関数：<=
    //    GREATER,           // 比較関数：>
    //    NOT_EQUAL,         // 比較関数：!=
    //    GREATER_EQUAL,     // 比較関数：>=
    //    ALWAYS,            // 比較関数：常に真
    //};

    /* BlendState */

    enum class BlendMode {
        NO,                // ブレンドしない
        ALPHA,             // アルファブレンド
        ADD,               // 加算
    };
    //enum class BlendOp {
    //    ADD,               // 加算
    //    SUBTRACT,          // 減算
    //    REV_SUBTRACT,      // 逆減算
    //    MIN,               // 最小
    //    MAX,               // 最大
    //};

    /* Topology */

    //enum class PrimitiveTopology {
    //    UNDEFINED,         // 未定義
    //    POINTLIST,         // ポイントリスト
    //    LINELIST,          // ラインリスト
    //    LINESTRIP,         // ラインストリップ
    //    TRIANGLELIST,      // トライアングルリスト
    //    TRIANGLESTRIP,     // トライアングルストリップ
    //    LINELIST_ADJ,      // ラインリスト（隣接）
    //    LINESTRIP_ADJ,     // ラインストリップ（隣接）
    //    TRIANGLELIST_ADJ,  // トライアングルリスト（隣接）
    //    TRIANGLESTRIP_ADJ, // トライアングルストリップ（隣接）
    //    PATCHLIST,         // パッチリスト
    //};


    /**************************************************
    * Setup desc
    **************************************************/

    inline void SetUpModeDesc(DXGI_MODE_DESC* md, const std::pair<int32_t, int32_t>& size, ID3D11Device* dev, IDXGIAdapter* adapter) {
        SecureZeroMemory(md, sizeof(*md));

        md->Width                   = size.first;                          // バッファの横幅
        md->Height                  = size.second;                         // バッファの縦幅
        md->RefreshRate.Numerator   = 60;                                  // リフレッシュレートの分母
        md->RefreshRate.Denominator = 1;                                   // リフレッシュレートの分子
        md->Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;          // カラーフォーマットの指定
        md->ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;// スキャンライン (走査線)の指定
        // ウィンドウ描画時のスケーリングの指定バックバッファのサイズ通りに描画したりウィンドウサイズに合わせるなど指定可能
        md->Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;

        ComPtr<IDXGIOutput> output = nullptr;
        DXGI_MODE_DESC matched_md{};
        if (DXGI_ERROR_NOT_FOUND != adapter->EnumOutputs(0, &output)) {
            if (FAILED(output->FindClosestMatchingMode(md, &matched_md, dev))) {
                assert::ShowWarning(ASSERT_FILE_LINE, "Get display mode failed");
            }
        }
        else {
            assert::ShowWarning(ASSERT_FILE_LINE, "Adapter output not found");
        }
        //*md = matched_md;
    }

    inline void SetUpSampleDesc(DXGI_SAMPLE_DESC* sd, ID3D11Device* dev) {
        SecureZeroMemory(sd, sizeof(*sd));

        sd->Count   = 1; // マルチサンプリングのサンプル数 (未使用は1)
        sd->Quality = 0; // マルチサンプリングの品質 (未使用は0)

        for (UINT i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1) {
            UINT quality = 0;
            if (SUCCEEDED(dev->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &quality))) {
                if (0 < quality) {
                    sd->Count   = i;
                    sd->Quality = quality - 1;
                }
            }
        }
    }

    inline void SetUpSwapChainDesc(DXGI_SWAP_CHAIN_DESC* scd, const std::pair<int32_t, int32_t>& size, ID3D11Device* dev, IDXGIAdapter* adapter, HWND hWnd, bool enable_msaa, UINT buffer_count = 1, DXGI_SWAP_EFFECT se = DXGI_SWAP_EFFECT_DISCARD) {
        SecureZeroMemory(scd, sizeof(*scd));

        /* SwapChain設定 (ウィンドウのサイズやフォーマット) */
        SetUpModeDesc(&scd->BufferDesc, size, dev, adapter); // バックバッファの設定
        if (enable_msaa) {
            SetUpSampleDesc(&scd->SampleDesc, dev);          // マルチサンプリングの設定
        }
        else {
            scd->SampleDesc.Count   = 1;
            scd->SampleDesc.Quality = 0;
        }
        scd->BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT; // バッファの使用方法
        scd->BufferCount  = buffer_count;                                              // バッファの数
        scd->OutputWindow = hWnd;                                                      // 出力対象のウィンドウハンドル
        scd->Windowed     = TRUE;                                                      // ウィンドウモード or フルスクリーンモード
        scd->SwapEffect   = se;                                                        // SwapEffectの指定
        scd->Flags        = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;                    // SwapChainの設定フラグの指定
    }

    
    /**************************************************
    * Shader compile helper
    **************************************************/

    inline ID3DBlob* CompileShaderFromFile(std::string_view file_path, std::string_view entry_point, ShaderTarget target, bool enable_debug) {
        ID3DBlob* blob  = nullptr;
        ID3DBlob* error = nullptr;

        UINT compile_flag = 0;
        if (enable_debug) {
            compile_flag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
        }

        std::string target_str;
        switch (target) {
        case ShaderTarget::CS:
            target_str = "cs_5_0";
            break;
        case ShaderTarget::DS:
            target_str = "ds_5_0";
            break;
        case ShaderTarget::GS:
            target_str = "gs_5_0";
            break;
        case ShaderTarget::HS:
            target_str = "hs_5_0";
            break;
        case ShaderTarget::PS:
            target_str = "ps_5_0";
            break;
        case ShaderTarget::VS:
            target_str = "vs_5_0";
            break;
        default:
            assert::ShowError(ASSERT_FILE_LINE, "Unknown shader target");
            break;
        }

        if (FAILED(D3DCompileFromFile(
            sjis_to_wide(file_path.data()).c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entry_point.data(),
            target_str.c_str(),
            compile_flag,
            0,
            &blob,
            &error
        ))) {
            if (error) {
                assert::ShowError(ASSERT_FILE_LINE, std::string(file_path) + "Shader compile failed: " + std::string(static_cast<char*>(error->GetBufferPointer()), error->GetBufferSize()));
            }
            else {
                assert::ShowError(ASSERT_FILE_LINE, "Shader compile failed");
            }
        }

        return blob;
    }


    /**************************************************
    * State helper
    **************************************************/

    inline ID3D11DepthStencilState* CreateDepthStencilState(ID3D11Device* dev, bool enable_depth, bool enable_write_depth) {
        D3D11_DEPTH_STENCIL_DESC dsd{};
        dsd.DepthEnable      = enable_depth;                // 深度テストを有効にするか
        dsd.DepthWriteMask   = enable_write_depth ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO; // 深度バッファへの書き込みを許可するか
        dsd.DepthFunc        = D3D11_COMPARISON_LESS_EQUAL; // 深度テストの比較関数
        dsd.StencilEnable    = FALSE;                       // ステンシルテストを有効にするか
        dsd.StencilReadMask  = 0;                           // ステンシルバッファの読み込みマスク
        dsd.StencilWriteMask = 0;                           // ステンシルバッファの書き込みマスク
        /*
        dsd.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;   // ステンシルテストに失敗したときのステンシルバッファへの書き込み
        dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;   // 深度テストに失敗したときのステンシルバッファへの書き込み
        dsd.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;   // ステンシルテストに成功したときのステンシルバッファへの書き込み
        dsd.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS; // ステンシルテストの比較関数
        dsd.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;   // ステンシルテストに失敗したときのステンシルバッファへの書き込み
        dsd.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
        */
        ID3D11DepthStencilState* state = nullptr;
        if (FAILED(dev->CreateDepthStencilState(&dsd, &state))) {
            return nullptr;
        }
        return state;
    }

    inline ID3D11RasterizerState* CreateRasterizerState(ID3D11Device* dev, FillMode fill_mode, CullMode cull_mode, bool depth_clip_enable, bool scissor_enable) {
        D3D11_RASTERIZER_DESC rd{};

        switch (fill_mode) {
        case FillMode::SOLID:
            rd.FillMode = D3D11_FILL_SOLID;
            break;
        case FillMode::WIREFRAME:
            rd.FillMode = D3D11_FILL_WIREFRAME;
            break;
        }

        switch (cull_mode) {
        case CullMode::NONE:
            rd.CullMode = D3D11_CULL_NONE;
            break;
        case CullMode::FRONT:
            rd.CullMode = D3D11_CULL_FRONT;
            break;
        case CullMode::BACK:
            rd.CullMode = D3D11_CULL_BACK;
            break;
        }

        rd.FrontCounterClockwise = FALSE;             // カウンタクロックワイズか
        rd.DepthBias             = 0;                 // 深度バイアス
        rd.DepthBiasClamp        = 0.0f;              // 深度バイアスのクランプ値
        rd.SlopeScaledDepthBias  = 0.0f;              // スロープスケール深度バイアス
        rd.DepthClipEnable       = depth_clip_enable; // 深度クリップを有効にするか
        rd.ScissorEnable         = scissor_enable;    // スキッシュテストを有効にするか
        rd.MultisampleEnable     = FALSE;             // マルチサンプリングを有効にするか
        rd.AntialiasedLineEnable = FALSE;             // アンチエイリアスを有効にするか

        ID3D11RasterizerState* state = nullptr;
        if (FAILED(dev->CreateRasterizerState(&rd, &state))) {
            return nullptr;
        }
        return state;
    }

    inline ID3D11SamplerState* CreateSamplerState(ID3D11Device* dev, SamplerFilterMode sampler_filter_mode, UINT max_anisotropy, SamplerAddressMode sampler_address_mode, bool comparison_model) {
        D3D11_SAMPLER_DESC sd{};

        if (comparison_model) {
            switch (sampler_filter_mode) {
            case SamplerFilterMode::POINT:
                sd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
                break;
            case SamplerFilterMode::LINEAR:
                sd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
                break;
            case SamplerFilterMode::ANISOTROPIC:
                sd.Filter = D3D11_FILTER_COMPARISON_ANISOTROPIC;
                break;
            }
            sd.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
        }
        else {
            switch (sampler_filter_mode) {
            case SamplerFilterMode::POINT:
                sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
                break;
            case SamplerFilterMode::LINEAR:
                sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                break;
            case SamplerFilterMode::ANISOTROPIC:
                sd.Filter = D3D11_FILTER_ANISOTROPIC;
                break;
            }
            sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        }

        switch (sampler_address_mode) {
        case SamplerAddressMode::WRAP:
            sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            break;
        case SamplerAddressMode::MIRROR:
            sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
            sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
            sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
            break;
        case SamplerAddressMode::CLAMP:
            sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
            break;
        case SamplerAddressMode::BORDER:
            sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
            sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
            sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
            break;
        case SamplerAddressMode::MIRROR_ONCE:
            sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
            sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
            sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
            break;
        }

        sd.MipLODBias     = 0;
        sd.MaxAnisotropy  = max_anisotropy;
        sd.BorderColor[0] = sd.BorderColor[1] = sd.BorderColor[2] = sd.BorderColor[3] = 0;
        sd.MinLOD         = 0;
        sd.MaxLOD         = D3D11_FLOAT32_MAX;

        ID3D11SamplerState* state = nullptr;
        if (FAILED(dev->CreateSamplerState(&sd, &state))) {
            return nullptr;
        }
        return state;
    }

    inline ID3D11BlendState* CreateBlendState(ID3D11Device* dev, BlendMode blend_mode) {
        D3D11_BLEND_DESC bd{};

        bd.AlphaToCoverageEnable                 = FALSE;
        bd.IndependentBlendEnable                = FALSE;
        bd.RenderTarget[0].BlendEnable           = TRUE; // ブレンドを有効にするかどうか
        bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // レンダーターゲットへの書き込みマスク

        // ブレンドなし
        if (blend_mode == BlendMode::NO) {
            // 色の合成方法
            bd.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
            bd.RenderTarget[0].SrcBlend       = D3D11_BLEND_ONE;
            bd.RenderTarget[0].DestBlend      = D3D11_BLEND_ZERO;
            // アルファの合成方法
            bd.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
            bd.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
            bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        }
        // 半透明ブレンド
        else if (blend_mode == BlendMode::ALPHA) {
            // 色の合成方法
            bd.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
            bd.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
            bd.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
            // アルファの合成方法
            bd.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
            bd.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
            bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        }
        // 加算ブレンド
        else if (blend_mode == BlendMode::ADD) {
            // 色の合成方法
            bd.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
            bd.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
            bd.RenderTarget[0].DestBlend      = D3D11_BLEND_ONE;
            // アルファの合成方法
            bd.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
            bd.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
            bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        }

        ID3D11BlendState* state = nullptr;
        if (FAILED(dev->CreateBlendState(&bd, &state))) {
            return nullptr;
        }
        return state;
    }


    /**************************************************
    * Texture helper
    **************************************************/

    inline D3D11_TEXTURE2D_DESC GetTextureInfo(ID3D11View* view) {
        D3D11_TEXTURE2D_DESC td{};
        ID3D11Resource*      resource  = nullptr;
        ID3D11Texture2D*     texture2d = nullptr;

        SecureZeroMemory(&td, sizeof(td));
        view->GetResource(&resource);
        if (SUCCEEDED(resource->QueryInterface<ID3D11Texture2D>(&texture2d))) {
            texture2d->GetDesc(&td);
            memory::SafeRelease(&texture2d);
        }
        memory::SafeRelease(&resource);
        return td;
    }

    inline bool CreateTexture2D(ID3D11Device* dev, ID3D11Texture2D* resource, ID3D11ShaderResourceView** srv, ID3D11RenderTargetView** rtv, ID3D11DepthStencilView** dsv) {
        D3D11_TEXTURE2D_DESC td{};
        SecureZeroMemory(&td, sizeof(td));

        resource->GetDesc(&td);

        if (rtv && td.BindFlags & D3D11_BIND_RENDER_TARGET) {
            D3D11_RENDER_TARGET_VIEW_DESC rtvd{};
            SecureZeroMemory(&rtvd, sizeof(rtvd));

            rtvd.Format = td.Format;
            if (td.ArraySize == 1) {
                rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            }
            else {
                rtvd.ViewDimension                  = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
                rtvd.Texture2DArray.MipSlice        = 0;
                rtvd.Texture2DArray.FirstArraySlice = 0;
                rtvd.Texture2DArray.ArraySize       = td.ArraySize;
            }

            if (FAILED(dev->CreateRenderTargetView(resource, &rtvd, rtv))) {
                return false;
            }
        }

        if (td.BindFlags & D3D11_BIND_DEPTH_STENCIL) {
            D3D11_DEPTH_STENCIL_VIEW_DESC dsvd{};
            SecureZeroMemory(&dsvd, sizeof(dsvd));

            switch (td.Format) {
            case DXGI_FORMAT_R16_TYPELESS:
                dsvd.Format = DXGI_FORMAT_D16_UNORM;
                break;
            case DXGI_FORMAT_R32_TYPELESS:
                dsvd.Format = DXGI_FORMAT_D32_FLOAT;
                break;
            case DXGI_FORMAT_R24G8_TYPELESS:
                dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                break;
            default:
                break;
            }

            if (td.ArraySize == 1) {
                dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                dsvd.Texture2D.MipSlice = 0;
            }
            else {
                dsvd.ViewDimension                  = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
                dsvd.Texture2DArray.MipSlice        = 0;
                dsvd.Texture2DArray.FirstArraySlice = 0;
                dsvd.Texture2DArray.ArraySize       = td.ArraySize;
            }

            if (FAILED(dev->CreateDepthStencilView(resource, &dsvd, dsv))) {
                return false;
            }
        }

        if (srv && td.BindFlags & D3D11_BIND_SHADER_RESOURCE) {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
            SecureZeroMemory(&srvd, sizeof(srvd));

            if (td.BindFlags & D3D11_BIND_RENDER_TARGET) {
                srvd.Format = td.Format;
            }
            if (td.BindFlags & D3D11_BIND_DEPTH_STENCIL) {
                switch (td.Format) {
                case DXGI_FORMAT_R16_TYPELESS:
                    srvd.Format = DXGI_FORMAT_R16_UNORM;
                    break;
                case DXGI_FORMAT_R32_TYPELESS:
                    srvd.Format = DXGI_FORMAT_R32_FLOAT;
                    break;
                case DXGI_FORMAT_R24G8_TYPELESS:
                    srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
                    break;
                default:
                    break;
                }
            }
            if (td.ArraySize == 1) {
                srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                srvd.Texture2D.MostDetailedMip = 0;
                srvd.Texture2D.MipLevels = td.MipLevels;
                if (srvd.Texture2D.MipLevels <= 0) {
                    srvd.Texture2D.MipLevels = static_cast<UINT>(-1);
                }
            }
            else {
                if (td.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) {
                    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
                }
                else {
                    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                }
                srvd.Texture2DArray.MostDetailedMip = 0;
                srvd.Texture2DArray.MipLevels = td.MipLevels;
                srvd.Texture2DArray.ArraySize = td.ArraySize;
                srvd.Texture2DArray.FirstArraySlice = 0;
            }

            if (FAILED(dev->CreateShaderResourceView(resource, &srvd, srv))) {
                return false;
            }
        }

        return true;
    }

}

#endif
