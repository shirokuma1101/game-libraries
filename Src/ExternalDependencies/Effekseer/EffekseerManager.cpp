#include "EffekseerManager.h"

#include <ExternalDependencies/Utility/String.h>

void EffekseerManager::Init(ID3D11Device& dev, ID3D11DeviceContext& ctx, int max_square)
{
    m_maxSquare = max_square;
    effekseer_helper::RendererInit(&m_renderer, dev, ctx, m_maxSquare);
    effekseer_helper::ManagerInit(&m_manager, m_renderer, m_maxSquare);
}

void EffekseerManager::Update(double delta_time)
{
    for (auto iter = m_emittedEffectData.begin(); iter != m_emittedEffectData.end();) {
        auto& key      = iter->first;
        auto& data     = iter->second;
        auto& handle   = iter->second.handle;
        auto& tranform = iter->second.effectTransform;

        if (data.elapsedTime == 0) {
            handle = m_manager->Play(data.effect, 0, 0, 0);
            m_manager->SetMatrix(handle, effekseer_helper::ToMatrix43(tranform.matrix));
            m_manager->SetSpeed(handle, tranform.speed);
        }

        if (static_cast<float>(data.elapsedTime) > (tranform.maxFrame / 60.f)) {
            m_manager->StopEffect(handle);
            if (tranform.isLoop) {
                data.elapsedTime = 0;
            }
            else {
                iter = m_emittedEffectData.erase(iter);
            }
        }
        else {
            m_renderer->SetTime(static_cast<float>(data.elapsedTime));
            data.elapsedTime += delta_time;
            ++iter;
        }
    }

    m_manager->Update();
}

void EffekseerManager::Draw()
{
    m_renderer->BeginRendering();
    m_manager->Draw();
    m_renderer->EndRendering();
}

void EffekseerManager::SetCamera(const DirectX::SimpleMath::Matrix& proj_mat, const DirectX::SimpleMath::Matrix& view_mat)
{
    m_renderer->SetProjectionMatrix(effekseer_helper::ToMatrix44(proj_mat));
    m_renderer->SetCameraMatrix(effekseer_helper::ToMatrix44(view_mat));
}

void EffekseerManager::SetEffect(std::string_view effect_name, std::string_view file_path)
{
    effekseer_helper::EffectData effect_data;
    effect_data.effect = Effekseer::Effect::Create(m_manager, string::StrToUtf16(file_path).c_str());
    m_effectData.emplace(effect_name, effect_data);
}

effekseer_helper::EffectTransform& EffekseerManager::Emit(std::string_view effect_name, const effekseer_helper::EffectTransform& effect_transform, bool is_unique)
{
    if (is_unique) {
        if (m_emittedEffectData.count(effect_name.data())) return;
    }
    if (auto iter = m_effectData.find(effect_name.data()); iter != std::end(m_effectData)) {
        auto data = effekseer_helper::EffectData(iter->second);
        data.effectTransform = effekseer_helper::EffectTransform(effect_transform);
        m_emittedEffectData.emplace(effect_name, data);
        return data.effectTransform;
    }
}

void EffekseerManager::Release() noexcept
{
    m_manager.Reset();
    m_renderer.Reset();
}
