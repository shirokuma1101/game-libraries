#pragma once

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

    std::tuple<bool, bool, bool, bool, bool, bool> GetDirection(std::tuple<int, int, int, int, int, int> direction, bool is_press_and_hold = true) noexcept {
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

class MouseManager
{
public:

    MouseManager(HWND hwnd = 0)
        : m_mouseData(hwnd)
    {}

    void Update() {
        m_mouseData.GetPosition();
    }

    POINT GetPosition() {
        return m_mouseData.GetPoint();
    }

    POINT GetDifference() {
        return m_mouseData.GetDifference();
    }

private:

    input_helper::MouseData m_mouseData;

};

class InputManager
{
public:

    InputManager(HWND hwnd = 0)
        : m_spKeyMgr(std::make_shared<KeyManager>())
        , m_spMouseMgr(std::make_shared<MouseManager>())
    {}

    void Update() {
        m_spKeyMgr->Update();
        m_spMouseMgr->Update();
    }

    auto GetKeyManager() {
        return m_spKeyMgr;
    }

    auto GetMouseManager() {
        return m_spMouseMgr;
    }

private:

    std::shared_ptr<KeyManager> m_spKeyMgr;
    std::shared_ptr<MouseManager> m_spMouseMgr;
    
};
