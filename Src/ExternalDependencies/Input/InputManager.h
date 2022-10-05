#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_INPUTMANAGER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_INPUTMANAGER_H_

#include <list>
#include <memory>
#include <tuple>

#include "InputHelper.h"

class KeyManager
{
public:

    static constexpr int PRESS = 1;
    static constexpr int RELEASE = 2;

    enum class KeyState {
        None    = 1 << 0, // No pressed
        Press   = 1 << 1, // Just press
        Hold    = 1 << 2, // Pushed and hold
        Release = 1 << 3, // Just release
    };
    friend bool operator&(KeyState lhs, KeyState rhs) {
        using UnderlyingTypeT = std::underlying_type_t<KeyState>;
        return (static_cast<UnderlyingTypeT>(lhs) & static_cast<UnderlyingTypeT>(rhs));
    }
    friend bool operator|(KeyState lhs, KeyState rhs) {
        using UnderlyingTypeT = std::underlying_type_t<KeyState>;
        return (static_cast<UnderlyingTypeT>(lhs) | static_cast<UnderlyingTypeT>(rhs));
    }

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

    bool GetState(int key, KeyState key_state = KeyState::Hold) noexcept {
        for (const auto & e : m_keys) {
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
        if (key_state & KeyState::Hold) {
            state |= true;
        }
        if (key_state & KeyState::Press) {
            state |= std::get<PRESS>(key);
        }
        if (key_state & KeyState::Release) {
            state |= std::get<RELEASE>(key);
        }
        return state;
    }

    // { KeyData, Press, Release }
    std::list<std::tuple<input_helper::KeyData, bool, bool>> m_keys;
    
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
