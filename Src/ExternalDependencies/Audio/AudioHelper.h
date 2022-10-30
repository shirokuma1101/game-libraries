#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_AUDIO_AUDIOHELPER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_AUDIO_AUDIOHELPER_H_

#include <memory>
#include <string>
#include <string_view>

#include "Utility/Assert.h"

#include "Audio.h"
#include "SimpleMath.h"
#include "strconv.h"

#ifndef AUDIOHELPER_DISABLE_LINK_LIBS
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTKAudioWin8.lib")
#endif

namespace audio_helper {
    
    class SoundData
    {
    public:

        SoundData(std::string_view file_path) noexcept
            : m_filePath(file_path)
        {}
        SoundData(DirectX::AudioEngine* engine, std::string_view file_path)
            : m_filePath(file_path)
        {
            Load(engine);
        }

        bool Load(DirectX::AudioEngine* engine) {
            try {
                m_upSoundEffect = std::make_unique<DirectX::SoundEffect>(engine, sjis_to_wide(m_filePath.data()).c_str());
                return true;
            }
            catch (...) {
                assert::RaiseAssert(ASSERT_FILE_LINE, "Failed to load sound effect.");
                return false;
            }
        }

        std::unique_ptr<DirectX::SoundEffectInstance> CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags) {
            if (!m_upSoundEffect) {
                return nullptr;
            }

            return m_upSoundEffect->CreateInstance(flags);
        }

    private:

        const std::string                     m_filePath;
        std::unique_ptr<DirectX::SoundEffect> m_upSoundEffect = nullptr;
        
    };

    class SoundInstance
    {
    public:

        SoundInstance(const std::shared_ptr<SoundData> sound_data) noexcept
            : m_spSoundData(sound_data)
        {}
        SoundInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags, const std::shared_ptr<SoundData> sound_data)
            : m_spSoundData(sound_data)
        {
            CreateInstance(flags);
        }

        bool CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags) {
            if (!m_spSoundData) {
                return false;
            }
            m_upSoundEffectInstance = m_spSoundData->CreateInstance(flags);
            if (flags & DirectX::SOUND_EFFECT_INSTANCE_FLAGS::SoundEffectInstance_Use3D) {
                m_upAudioEmitter = std::make_unique<DirectX::AudioEmitter>();
            }
            return static_cast<bool>(m_upSoundEffectInstance);
        }

        std::shared_ptr<const SoundData> GetSoundData() const noexcept {
            return m_spSoundData;
        }

        const std::unique_ptr<DirectX::SoundEffectInstance>& GetSoundEffectInstance() const noexcept {
            return m_upSoundEffectInstance;
        }

        /**************************************************
        * DirectX::SoundEffectInstance function wrap
        **************************************************/
        
        void Play(bool loop = false) noexcept {
            if (!m_upSoundEffectInstance) return;
            m_upSoundEffectInstance->Play(loop);
        }
        void Stop(bool immediate = true) noexcept {
            if (!m_upSoundEffectInstance) return;
            m_upSoundEffectInstance->Stop(immediate);
        }
        void Pause() noexcept {
            if (!m_upSoundEffectInstance) return;
            m_upSoundEffectInstance->Pause();
        }
        void Resume() noexcept {
            if (!m_upSoundEffectInstance) return;
            m_upSoundEffectInstance->Resume();
        }

        void SetVolume(float volume) noexcept {
            if (!m_upSoundEffectInstance) return;
            m_upSoundEffectInstance->SetVolume(volume);
        }
        void SetPitch(float pitch) noexcept {
            if (!m_upSoundEffectInstance) return;
            m_upSoundEffectInstance->SetPitch(pitch);
        }
        void SetPan(float pan) noexcept {
            if (!m_upSoundEffectInstance) return;
            m_upSoundEffectInstance->SetPan(pan);
        }

        void Apply3D(const DirectX::AudioListener& listener, const DirectX::SimpleMath::Vector3& position, bool rhcoords = false) {
            if (!m_upSoundEffectInstance) return;
            m_upAudioEmitter->SetPosition(position);
            m_upSoundEffectInstance->Apply3D(listener, *m_upAudioEmitter, rhcoords);
        }
        
        bool IsLooped() const noexcept {
            if (!m_upSoundEffectInstance) return false;
            return m_upSoundEffectInstance->IsLooped();
        }
        
        bool IsState(DirectX::SoundState state) const noexcept {
            if (!m_upSoundEffectInstance) {
                return false;
            }
            return (m_upSoundEffectInstance->GetState() == state);
        }

        unsigned int GetChannelCount() const noexcept {
            if (!m_upSoundEffectInstance) {
                return 0;
            }
            return m_upSoundEffectInstance->GetChannelCount();
        }

        DirectX::IVoiceNotify* GetVoiceNotify() const noexcept {
            if (!m_upSoundEffectInstance) {
                return nullptr;
            }
            return m_upSoundEffectInstance->GetVoiceNotify();
        }

    private:

        const std::shared_ptr<SoundData>              m_spSoundData           = nullptr;
        std::unique_ptr<DirectX::SoundEffectInstance> m_upSoundEffectInstance = nullptr;
        std::unique_ptr<DirectX::AudioEmitter>        m_upAudioEmitter        = nullptr;
        
    };
    
}

#endif
