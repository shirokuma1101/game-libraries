#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "SimpleMath.h"
#include "Effekseer.h" // Effekseer must set both "include" and "Effekseer" directory
#include "EffekseerRendererDX11/EffekseerRendererDX11.h"

namespace effekseer_helper {

    using Effect   = Effekseer::EffectRef;
    using Handle   = Effekseer::Handle;
    using Renderer = EffekseerRendererDX11::RendererRef;
    using Manager  = Effekseer::ManagerRef;

    struct EffectTransform {
        DirectX::SimpleMath::Matrix matrix;
        bool                        isLoop   = false;
        float                       speed    = 1.f;
        int                         maxFrame = 0;
    };

    struct EffectData {
        EffectData() = default;
        EffectData(const EffectData& effect_data)
            : effect(effect_data.effect)
        {}
        
        Effect          effect;
        Handle          handle       = 0;
        double          elapsedTime  = 0;
        EffectTransform effectTransform;
    };

    inline void RendererInit(Renderer* renderer, ID3D11Device& dev, ID3D11DeviceContext& ctx, int max_square) {
        *renderer = EffekseerRendererDX11::Renderer::Create(&dev, &ctx, max_square);
    }

    inline void ManagerInit(Manager* manager, const Renderer& renderer, int max_square) {
        *manager = Effekseer::Manager::Create(max_square);
        (*manager)->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
        (*manager)->SetSpriteRenderer(renderer->CreateSpriteRenderer());
        (*manager)->SetRibbonRenderer(renderer->CreateRibbonRenderer());
        (*manager)->SetRingRenderer(renderer->CreateRingRenderer());
        (*manager)->SetTrackRenderer(renderer->CreateTrackRenderer());
        (*manager)->SetModelRenderer(renderer->CreateModelRenderer());
        (*manager)->SetTextureLoader(renderer->CreateTextureLoader());
        (*manager)->SetModelLoader(renderer->CreateModelLoader());
        (*manager)->SetMaterialLoader(renderer->CreateMaterialLoader());
        (*manager)->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
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
