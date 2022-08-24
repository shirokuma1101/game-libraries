#pragma once

#include "EffekseerHelper.h"

class EffekseerManager {
public:

    ~EffekseerManager() noexcept { Release(); }

    void Init(ID3D11Device* dev, ID3D11DeviceContext* ctx, int max_square = 8192);
    void Update();
    void Draw();

    void SetCamera(const DirectX::SimpleMath::Matrix& proj_mat, const DirectX::SimpleMath::Matrix& view_mat);
    void SetEffect(std::string_view effect_name, std::string_view file_path);
    
    void Emit(std::string_view effect_name, const effekseer_helper::EffectTransform& effect_transform, bool is_unique = false);

private:

    void Release() noexcept {
        m_manager.Reset();
        m_renderer.Reset();
    };

    int                                                                m_maxSquare = 0;
    effekseer_helper::Renderer                                         m_renderer;
    effekseer_helper::Manager                                          m_manager;
    std::unordered_map<std::string, effekseer_helper::EffectData>      m_effectData;
    std::unordered_multimap<std::string, effekseer_helper::EffectData> m_emittedEffectData;

};
