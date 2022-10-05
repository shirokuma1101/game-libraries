#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_EFFEKSEER_EFFEKSEERMANAGER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_EFFEKSEER_EFFEKSEERMANAGER_H_

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "EffekseerHelper.h"

class EffekseerManager
{
public:

    virtual ~EffekseerManager() noexcept {
        Release();
    }

    void Init(ID3D11Device* dev, ID3D11DeviceContext* ctx, int max_square = 8192) {
        effekseer_helper::RendererInit(&m_rendererRef, dev, ctx, max_square);
        effekseer_helper::ManagerInit(&m_managerRef, m_rendererRef, max_square);
    }

    void Update(double delta_time) {
        static const double effect_frame = 60.0;
        for (auto iter = m_upEffectInstances.begin(); iter != m_upEffectInstances.end();) {
            auto& data     = *iter->second;
            auto& handle   = iter->second->handle;
            auto& tranform = iter->second->effectTransform;

            if (data.elapsedTime == 0) {
                handle = m_managerRef->Play(data.GetEffectData()->GetEffectRef(), 0, 0, 0);
            }

            if (data.elapsedTime > (tranform->maxFrame / effect_frame)) {
                m_managerRef->StopEffect(handle);
                if (tranform->isLoop) {
                    data.elapsedTime = 0;
                }
                else {
                    iter = m_upEffectInstances.erase(iter);
                }
            }
            else {
                m_rendererRef->SetTime(static_cast<float>(data.elapsedTime));
                m_managerRef->SetMatrix(handle, effekseer_helper::ToMatrix43(tranform->matrix));
                m_managerRef->SetSpeed(handle, tranform->speed);
                data.elapsedTime += delta_time;
                ++iter;
            }
        }

        m_managerRef->Update(static_cast<float>(delta_time * effect_frame));
    }
    
    void Draw() {
        m_rendererRef->BeginRendering();
        m_managerRef->Draw();
        m_rendererRef->EndRendering();
    }

    void SetCamera(const DirectX::SimpleMath::Matrix& proj_mat, const DirectX::SimpleMath::Matrix& view_mat) {
        m_rendererRef->SetProjectionMatrix(effekseer_helper::ToMatrix44(proj_mat));
        m_rendererRef->SetCameraMatrix(effekseer_helper::ToMatrix44(view_mat));
    }

    void SetEffect(std::string_view effect_name, std::string_view file_path) {
        m_spEffectData.emplace(effect_name, std::make_shared<effekseer_helper::EffectData>(m_managerRef, file_path));
    }
    
    std::shared_ptr<effekseer_helper::EffectTransform> Emit(std::string_view effect_name, const effekseer_helper::EffectTransform& effect_transform, bool is_unique = false) {
        if (is_unique) {
            if (auto iter = m_upEffectInstances.find(effect_name.data()); iter != m_upEffectInstances.end()) {
                return iter->second->effectTransform;
            }
        }
        if (auto iter = m_spEffectData.find(effect_name.data()); iter != m_spEffectData.end()) {
            auto effect_instance = std::make_unique<effekseer_helper::EffectInstance>(iter->second);
            effect_instance->effectTransform = std::make_shared<effekseer_helper::EffectTransform>(effect_transform);
            auto& sp_et = effect_instance->effectTransform;
            m_upEffectInstances.emplace(effect_name, std::move(effect_instance));
            return sp_et;
        }
        return nullptr;
    }

private:

    void Release() noexcept {
        m_managerRef.Reset();
        m_rendererRef.Reset();
    }

    effekseer_helper::RendererRef                                                           m_rendererRef;
    Effekseer::ManagerRef                                                                   m_managerRef;
    std::unordered_map<std::string, std::shared_ptr<effekseer_helper::EffectData>>          m_spEffectData;
    std::unordered_multimap<std::string, std::unique_ptr<effekseer_helper::EffectInstance>> m_upEffectInstances;

};

#endif
