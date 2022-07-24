#include "EffekseerManager.h"

void EffekseerManager::Init(ID3D11Device* dev, ID3D11DeviceContext* ctx, int max_square)
{
	m_maxSquare = max_square;
	effekseer_helper::RendererInit(m_renderer, dev, ctx, m_maxSquare);
	effekseer_helper::ManagerInit(m_manager, m_renderer, m_maxSquare);
}

void EffekseerManager::Update()
{
	for (auto iter = m_emittedEffectData.begin(); iter != m_emittedEffectData.end();) {
		auto& handle = iter->second.handle;
		auto& key    = iter->first;
		auto& value  = iter->second;

		// �����t���[���̏ꍇ
		if (value.frame == 0) {
			// �J�n
			handle = m_manager->Play(value.effect, 0, 0, 0);
			m_manager->SetMatrix(handle, effekseer_helper::ToMatrix43(value.effectTransform.matrix));
			m_manager->SetSpeed(handle, value.effectTransform.speed);
		}

		// �t���[�����ő�t���[���𒴂����ꍇ
		if ((value.frame) > (value.effectTransform.maxFrame)) {
			// ��~
			m_manager->StopEffect(handle);
			// ���[�v�ł͂Ȃ��ꍇ
			if (!value.isLoop) {
				// �v�f���폜 (���̃C�e���[�^������)
				iter = m_emittedEffectData.erase(iter);
			}
			else {
				// �t���[������0�ɂ���
				value.frame = 0;
			}
		}
		else {
			// �t���[����i�߂�
			m_renderer->SetTime(value.frame / 60.0f);
			++(value.frame);
			// ���̗v�f�̃C�e���[�^
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
	//effect_data.effect = Effekseer::Effect::Create(m_manager, file_path.data());
	m_effectData.emplace(effect_name, effect_data);
}

void EffekseerManager::Emit(std::string_view effect_name, const effekseer_helper::EffectTransform& effect_transform, bool is_unique)
{
	if (is_unique) {
		if (m_emittedEffectData.count(effect_name.data())) return;
	}
	if (auto iter = m_effectData.find(effect_name.data()); iter != std::end(m_effectData)) {
		auto value = iter->second; // Copy
		value.effectTransform = effect_transform;
		m_emittedEffectData.emplace(effect_name, effekseer_helper::EffectData(value));
	}
}
