#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_AUDIO_AUDIOMANAGER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_AUDIO_AUDIOMANAGER_H_

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <Windows.h>

#include "Utility/Assert.h"
#include "ExternalDependencies/Audio/AudioHelper.h"

class AudioManager
{
public:

    enum class PlayFlags {
        None   = 1 << 0,
        Loop   = 1 << 1,
        Unique = 1 << 2,
        Play3D = 1 << 3,
    };
    friend bool operator&(PlayFlags lhs, PlayFlags rhs) {
        using UnderlyingTypeT = std::underlying_type_t<PlayFlags>;
        return (static_cast<UnderlyingTypeT>(lhs) & static_cast<UnderlyingTypeT>(rhs));
    }
    friend bool operator|(PlayFlags lhs, PlayFlags rhs) {
        using UnderlyingTypeT = std::underlying_type_t<PlayFlags>;
        return (static_cast<UnderlyingTypeT>(lhs) | static_cast<UnderlyingTypeT>(rhs));
    }

    virtual ~AudioManager() {
        Release();
    }

    void Init(DirectX::AUDIO_ENGINE_REVERB reverb_flag = DirectX::AUDIO_ENGINE_REVERB::Reverb_Default) {
        if (FAILED(CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED))) {
            assert::RaiseAssert(ASSERT_FILE_LINE, "Failed to initialize COM.");
        }
        
        DirectX::AUDIO_ENGINE_FLAGS flags = DirectX::AUDIO_ENGINE_FLAGS::AudioEngine_Default;
#ifdef _DEBUG
        flags |= DirectX::AUDIO_ENGINE_FLAGS::AudioEngine_Debug;
#endif
        m_upAudioEngine = std::make_unique<DirectX::AudioEngine>(flags);
        m_upAudioEngine->SetReverb(reverb_flag);
        m_listener.OrientFront = { 0.f, 0.f, 1.f };
    }

    void Update(const DirectX::SimpleMath::Vector3& position = {}, const DirectX::SimpleMath::Vector3& direction = {}) {
        if (!m_upAudioEngine->Update()) {
            if (m_upAudioEngine->IsCriticalError()) {
                assert::RaiseAssert(ASSERT_FILE_LINE, "Audio engine critical error.");
            }
        }
        m_listener.Position = position;
        m_listener.OrientFront = direction;

        for (auto iter = m_spSoundInstances.begin(); iter != m_spSoundInstances.end();) {
            if (iter->second->IsState(DirectX::SoundState::STOPPED)) {
                iter = m_spSoundInstances.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }

    std::shared_ptr<audio_helper::SoundInstance> GetSoundInstance(std::string_view sound_name) {
        if (auto iter = m_spSoundInstances.find(sound_name.data()); iter != m_spSoundInstances.end()) {
            return iter->second;
        }
        return nullptr;
    }

    void SetAudio(std::string_view sound_name, std::string_view file_path) {
        m_spSoundData.emplace(sound_name, std::make_shared<audio_helper::SoundData>(m_upAudioEngine.get(), file_path));
    }

    std::shared_ptr<audio_helper::SoundInstance> Play(std::string_view sound_name, PlayFlags play_flags = PlayFlags::None, const DirectX::SimpleMath::Vector3& position = {}) {
        if (play_flags & PlayFlags::Unique) {
            if (auto iter = m_spSoundInstances.find(sound_name.data()); iter != m_spSoundInstances.end()) {
                return iter->second;
            }
        }
        if (auto iter = m_spSoundData.find(sound_name.data()); iter != m_spSoundData.end()) {
            DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default;
            if (play_flags & PlayFlags::Play3D) {
                flags |= DirectX::SoundEffectInstance_Use3D | DirectX::SoundEffectInstance_ReverbUseFilters;
            }
            auto sound_instance = std::make_shared<audio_helper::SoundInstance>(flags, iter->second);
            
            if (play_flags & PlayFlags::Play3D) {
                sound_instance->Apply3D(m_listener, position);
            }
            sound_instance->Play(play_flags & PlayFlags::Loop);

            m_spSoundInstances.emplace(sound_name, sound_instance);
            return sound_instance;
        }
        return nullptr;
    }

    void StopAllSound() {
        for (auto&& e : m_spSoundInstances) {
            e.second->GetSoundEffectInstance()->Stop();
        }
    }

private:

    void Release() {
        CoUninitialize();
    }

    std::unique_ptr<DirectX::AudioEngine>                                              m_upAudioEngine = nullptr;
    DirectX::AudioListener                                                             m_listener;
    std::unordered_map<std::string, std::shared_ptr<audio_helper::SoundData>>          m_spSoundData;
    std::unordered_multimap<std::string, std::shared_ptr<audio_helper::SoundInstance>> m_spSoundInstances;

};

#endif
