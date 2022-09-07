#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_EFFEKSEER_EFFEKSEERMANAGER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_EFFEKSEER_EFFEKSEERMANAGER_H_

#include <memory>

#include "ExternalDependencies/Utility/String.h"
#include "EffekseerHelper.h"

class EffekseerManager
{
public:

    ~EffekseerManager() noexcept {
        Release();
    }

    void Init(ID3D11Device& dev, ID3D11DeviceContext& ctx, int max_square = 8192) {
        m_maxSquare = max_square;
        effekseer_helper::RendererInit(&m_renderer, dev, ctx, m_maxSquare);
        effekseer_helper::ManagerInit(&m_manager, m_renderer, m_maxSquare);
    }

    void Update(double delta_time) {
        static const double effect_frame = 60.0;
        for (auto iter = m_upEmittedEffectData.begin(); iter != m_upEmittedEffectData.end();) {
            auto& data = *iter->second;
            auto& handle = iter->second->handle;
            auto& tranform = iter->second->effectTransform;

            if (data.elapsedTime == 0) {
                handle = m_manager->Play(data.effect, 0, 0, 0);
                m_manager->SetMatrix(handle, effekseer_helper::ToMatrix43(tranform.matrix));
                m_manager->SetSpeed(handle, tranform.speed);
            }

            if (data.elapsedTime > (tranform.maxFrame / effect_frame)) {
                m_manager->StopEffect(handle);
                if (tranform.isLoop) {
                    data.elapsedTime = 0;
                }
                else {
                    iter = m_upEmittedEffectData.erase(iter);
                }
            }
            else {
                m_renderer->SetTime(static_cast<float>(data.elapsedTime));
                data.elapsedTime += delta_time;
                ++iter;
            }
        }

        m_manager->Update(static_cast<float>(delta_time * effect_frame));
    }
    
    void Draw() {
        m_renderer->BeginRendering();
        m_manager->Draw();
        m_renderer->EndRendering();
    }

    void SetCamera(const DirectX::SimpleMath::Matrix& proj_mat, const DirectX::SimpleMath::Matrix& view_mat) {
        m_renderer->SetProjectionMatrix(effekseer_helper::ToMatrix44(proj_mat));
        m_renderer->SetCameraMatrix(effekseer_helper::ToMatrix44(view_mat));
    }
    
    void SetEffect(std::string_view effect_name, std::string_view file_path) {
        effekseer_helper::EffectData effect_data;
        effect_data.effect = Effekseer::Effect::Create(m_manager, string::StrToUtf16(file_path).c_str());
        m_effectData.emplace(effect_name, effect_data);
    }
    
    effekseer_helper::EffectTransform* Emit(std::string_view effect_name, const effekseer_helper::EffectTransform& effect_transform, bool is_unique = false) {
        if (is_unique) {
            if (auto iter = m_upEmittedEffectData.find(effect_name.data()); iter != m_upEmittedEffectData.end()) {
                return &iter->second->effectTransform;
            }
        }
        if (auto iter = m_effectData.find(effect_name.data()); iter != std::end(m_effectData)) {
            auto data = std::make_unique<effekseer_helper::EffectData>(iter->second);
            auto et_ptr = &data->effectTransform;
            data->effectTransform = effect_transform;
            m_upEmittedEffectData.emplace(effect_name, std::move(data));
            return et_ptr;
        }
        return nullptr;
    }

private:

    void Release() noexcept {
        m_manager.Reset();
        m_renderer.Reset();
    }

    int                                                                                 m_maxSquare = 0;
    effekseer_helper::Renderer                                                          m_renderer;
    effekseer_helper::Manager                                                           m_manager;
    std::unordered_map<std::string, effekseer_helper::EffectData>                       m_effectData;
    std::unordered_multimap<std::string, std::unique_ptr<effekseer_helper::EffectData>> m_upEmittedEffectData;

};

#endif
