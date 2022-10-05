#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_EFFEKSEER_EFFEKSEERHELPER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_EFFEKSEER_EFFEKSEERHELPER_H_

#include <memory>

#include "ExternalDependencies/Utility/String.h"

#pragma warning(push)
#pragma warning(disable:4100) // 'identifier' : unreferenced formal parameter
#include "Effekseer.h" // Effekseer must set both "include" and "Effekseer" directory
#include "EffekseerRendererDX11/EffekseerRendererDX11.h"
#pragma warning(pop)
#ifndef EFFEKSEERHELPER_DISABLE_LINK_LIBS
#ifdef _DEBUG
#pragma comment(lib, "Effekseerd.lib")
#pragma comment(lib, "EffekseerRendererDX11d.lib")
#else
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX11.lib")
#endif
#endif
#include "SimpleMath.h"

namespace effekseer_helper {

    using RendererRef = EffekseerRendererDX11::RendererRef;
    
    inline Effekseer::Vector3D ToVector3D(const DirectX::SimpleMath::Vector3& vec3) {
        return Effekseer::Vector3D(vec3.x, vec3.y, vec3.z);
    }

    inline Effekseer::Matrix43 ToMatrix43(const DirectX::SimpleMath::Matrix& mat) {
        Effekseer::Matrix43 mat43{};
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 3; ++j) {
                mat43.Value[i][j] = mat.m[i][j];
            }
        }
        return mat43;
    }

    inline Effekseer::Matrix44 ToMatrix44(const DirectX::SimpleMath::Matrix& mat) {
        Effekseer::Matrix44 mat44{};
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                mat44.Values[i][j] = mat.m[i][j];
            }
        }
        return mat44;
    }

    inline void RendererInit(RendererRef* renderer, ID3D11Device* dev, ID3D11DeviceContext* ctx, int max_square) {
        *renderer = EffekseerRendererDX11::Renderer::Create(dev, ctx, max_square);
    }

    inline void ManagerInit(Effekseer::ManagerRef* manager, const RendererRef& renderer, int max_square) {
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

    class EffectData
    {
    public:

        EffectData(std::string_view file_path)
            : m_filePath(file_path)
        {}
        EffectData(const Effekseer::ManagerRef& manager, std::string_view file_path)
            : m_filePath(file_path)
        {
            Load(manager);
        }

        void Load(const Effekseer::ManagerRef& manager) {
            m_effectRef = Effekseer::Effect::Create(manager, string::StrToUtf16(m_filePath).c_str());
        }

        std::string GetFilePath() const noexcept {
            return m_filePath;
        }

        const Effekseer::EffectRef& GetEffectRef() const noexcept {
            return m_effectRef;
        }

    private:

        const std::string    m_filePath;
        Effekseer::EffectRef m_effectRef;

    };

    struct EffectTransform {
        DirectX::SimpleMath::Matrix matrix;
        bool                        isLoop   = false;
        float                       speed    = 1.f;
        int                         maxFrame = 0;
    };

    class EffectInstance
    {
    public:

        EffectInstance(const std::shared_ptr<EffectData>& effect_data)
            : m_spEffectData(effect_data)
        {}

        const std::shared_ptr<EffectData>& GetEffectData() const noexcept {
            return m_spEffectData;
        }

        double                           elapsedTime = 0;
        Effekseer::Handle                handle      = 0;
        std::shared_ptr<EffectTransform> effectTransform;

    private:

        const std::shared_ptr<EffectData> m_spEffectData;

    };

}

#endif
