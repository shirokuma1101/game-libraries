#pragma once

#include <string_view>
#include <unordered_map>

#include "Inc/DirectXTK/SimpleMath.h"
#include "Inc/Effekseer/Effekseer.h"
#include "Inc/Effekseer/EffekseerRendererDX11.h"

namespace effekseer_helper {

    using Effect   = Effekseer::EffectRef;
    using Handle   = Effekseer::Handle;
    using Renderer = EffekseerRendererDX11::RendererRef;
    using Manager  = Effekseer::ManagerRef;

    struct EffectTransform {
        DirectX::SimpleMath::Matrix matrix;
        float                       speed    = 1.f;
        int                         maxFrame = 0;
    };

    struct EffectData {
        bool            isLoop = false;
        Effect          effect;
        Handle          handle = 0;
        int             frame  = 0;
        EffectTransform effectTransform;
    };

    inline void RendererInit(Renderer& renderer, ID3D11Device* dev, ID3D11DeviceContext* ctx, int max_square) {
        renderer = EffekseerRendererDX11::Renderer::Create(dev, ctx, max_square);
    }

    inline void ManagerInit(Manager& manager, const Renderer& renderer, int max_square) {
        manager = Effekseer::Manager::Create(max_square);
        manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
        manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
        manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
        manager->SetRingRenderer(renderer->CreateRingRenderer());
        manager->SetTrackRenderer(renderer->CreateTrackRenderer());
        manager->SetModelRenderer(renderer->CreateModelRenderer());
        manager->SetTextureLoader(renderer->CreateTextureLoader());
        manager->SetModelLoader(renderer->CreateModelLoader());
        manager->SetMaterialLoader(renderer->CreateMaterialLoader());
        manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
    }

    inline Effekseer::Vector3D ToVector3D(const DirectX::SimpleMath::Vector3& vec3) {
        return Effekseer::Vector3D{ vec3.x, vec3.y, vec3.z };
    }

    inline Effekseer::Matrix44 ToMatrix44(const DirectX::SimpleMath::Matrix& mat) {
        Effekseer::Matrix44 emat{};
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                emat.Values[i][j] = mat.m[i][j];
            }
        }
        return emat;
    }

    inline Effekseer::Matrix43 ToMatrix43(const DirectX::SimpleMath::Matrix& mat) {
        Effekseer::Matrix43 emat{};
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 3; ++j) {
                emat.Value[i][j] = mat.m[i][j];
            }
        }
        return emat;
    }
}
