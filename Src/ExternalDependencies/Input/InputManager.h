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

    void Update() noexcept {
        for (auto iter = m_keys.begin(); iter != m_keys.end();) {
            std::get<bool>(*iter) = false;
            if (std::get<input_helper::KeyData>(*iter).GetState()) {
                ++iter;
            }
            else {
                iter = m_keys.erase(iter);
            }
        }
    }

    bool GetState(int key, bool is_press_and_hold = true) noexcept {
        for (const auto & e : m_keys) {
            if (std::get<input_helper::KeyData>(e).GetKey() == key) {
                if (is_press_and_hold || std::get<bool>(e)) {
                    return true;
                }
                return false;
            }
        }
        if (input_helper::KeyData::GetState(key)) {
            m_keys.push_back({ input_helper::KeyData(key), true });
            return true;
        }
        return false;
    }

    std::tuple<bool, bool, bool, bool, bool, bool> GetDirection(const std::tuple<int, int, int, int, int, int>& direction, bool is_press_and_hold = true) noexcept {
        return std::make_tuple(
            GetState(std::get<0>(direction), is_press_and_hold),
            GetState(std::get<1>(direction), is_press_and_hold),
            GetState(std::get<2>(direction), is_press_and_hold),
            GetState(std::get<3>(direction), is_press_and_hold),
            GetState(std::get<4>(direction), is_press_and_hold),
            GetState(std::get<5>(direction), is_press_and_hold)
        );
    }

private:
    
    std::list<std::pair<input_helper::KeyData, bool>> m_keys;
    
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
