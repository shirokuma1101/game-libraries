#pragma once

#include <list>

#include "InputHelper.h"

class KeyManager
{
public:

    void Update() {
        for (auto iter = m_keys.begin(); iter != m_keys.end();) {
            if (iter->GetState()) {
                ++iter;
            }
            else {
                iter = m_keys.erase(iter);
            }
        }
    }

    bool GetState(int key, bool is_press_and_hold) {
        for (auto iter = m_keys.begin(); iter != m_keys.end();) {
            if (iter->GetKey() == key) {
                if (is_press_and_hold) {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                ++iter;
            }
        }
        if (input_helper::KeyData::GetState(key)) {
            m_keys.push_back(input_helper::KeyData(key));
            return true;
        }
        return false;
    }

private:
    
    std::list<input_helper::KeyData> m_keys;
    
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
        : m_keyMgr()
        , m_mouseMgr(hwnd)
    {}

    void Update() {
        m_keyMgr.Update();
        m_mouseMgr.Update();
    }

    const KeyManager& GetKeyManager() const {
        return m_keyMgr;
    }

    const MouseManager& GetMouseManager() const {
        return m_mouseMgr;
    }

private:

    KeyManager m_keyMgr;
    MouseManager m_mouseMgr;
    
};
