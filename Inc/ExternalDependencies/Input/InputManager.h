#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_INPUTMANAGER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_INPUTMANAGER_H_

#include <list>
#include <memory>
#include <tuple>
#include <unordered_map>

#include "InputHelper.h"

class KeyManager
{
public:

    static constexpr int PRESS   = 1;
    static constexpr int RELEASE = 2;

    enum KeyState {
        KEYSTATE_NONE    = 1 << 0, // No pressed
        KEYSTATE_PRESS   = 1 << 1, // Just press
        KEYSTATE_HOLD    = 1 << 2, // Pushed and hold
        KEYSTATE_RELEASE = 1 << 3, // Just release
    };

    void Update() noexcept {
        for (auto iter = m_keys.begin(); iter != m_keys.end();) {
            std::get<PRESS>(*iter) = false;
            bool state = std::get<input_helper::KeyData>(*iter).GetState();
            if (state) {
                ++iter;
            }
            else if (!state && !std::get<RELEASE>(*iter)) {
                std::get<RELEASE>(*iter) = true;
                ++iter;
            }
            else {
                iter = m_keys.erase(iter);
            }
        }
    }

    bool GetState(int key, KeyState key_state = KEYSTATE_HOLD) noexcept {
        for (const auto& e : m_keys) {
            if (std::get<input_helper::KeyData>(e).GetKey() == key) {
                return GetState(key_state, e);
            }
        }
        if (input_helper::KeyData::GetState(key)) {
            auto tkey = std::make_tuple(input_helper::KeyData(key), true, false);
            m_keys.push_back(tkey);
            return GetState(key_state, tkey);
        }
        return false;
    }

private:

    bool GetState(KeyState key_state, const std::tuple<input_helper::KeyData, bool, bool>& key) noexcept {
        bool state = false;
        if (key_state & KEYSTATE_HOLD) {
            state |= true;
        }
        if (key_state & KEYSTATE_PRESS) {
            state |= std::get<PRESS>(key);
        }
        if (key_state & KEYSTATE_RELEASE) {
            state |= std::get<RELEASE>(key);
        }
        return state;
    }

    // { KeyData, Press, Release }
    std::list<std::tuple<input_helper::KeyData, bool, bool>> m_keys;
    
};

template<class T>
class KeyConfigManager
{
public:

    void Update() noexcept {
        m_keyMgr.Update();
    }

    bool GetState(T config, KeyManager::KeyState key_state = KeyManager::KEYSTATE_HOLD) noexcept {
        if (auto iter = m_configuredKeys.find(config); iter != m_configuredKeys.end()) {
            return m_keyMgr.GetState(iter->second, key_state);
        }
        return false;
    }

    void AddKeyConfig(T config, int key) {
        m_configuredKeys.emplace(config, key);
    }
    void RemoveKeyConfig(T config) {
        for (auto iter = m_configuredKeys.begin(); iter != m_configuredKeys.end();) {
            if (iter->first == config) {
                iter = m_configuredKeys.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }
    void ResetKeyConfig() {
        m_configuredKeys.clear();
    }

private:

    KeyManager                 m_keyMgr;
    std::unordered_map<T, int> m_configuredKeys;

};

class InputManager
{
public:

    InputManager(HWND hwnd = 0)
        : m_upKeyMgr(std::make_unique<KeyManager>())
        , m_upCursorMgr(std::make_unique<input_helper::CursorData>(hwnd))
    {}

    void Update() noexcept {
        m_upKeyMgr->Update();
        m_upCursorMgr->GetPosition();
    }

    const std::unique_ptr<KeyManager>& GetKeyManager() const noexcept {
        return m_upKeyMgr;
    }

    const std::unique_ptr<input_helper::CursorData>& GetCursorManager() const noexcept {
        return m_upCursorMgr;
    }

private:

    std::unique_ptr<KeyManager>               m_upKeyMgr;
    std::unique_ptr<input_helper::CursorData> m_upCursorMgr;

};

#endif
