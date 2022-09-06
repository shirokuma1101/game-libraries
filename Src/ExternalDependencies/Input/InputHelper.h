#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_INPUTHELPER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_INPUTHELPER_H_

#include <Windows.h>

namespace input_helper {

    class KeyData
    {
    public:

        constexpr KeyData(int key) noexcept
            : m_key(key)
        {}

        int GetKey() const noexcept {
            return m_key;
        }

        bool GetState(bool is_press_and_hold = true) noexcept {
            if (GetState(m_key)) {
                if (is_press_and_hold) {
                    m_isPressed = true;
                    return true;
                }
                if (m_isPressed) {
                    return false;
                }
                return true;
            }
            m_isPressed = false;
            return false;
        }

        static bool GetState(int key) noexcept {
            return GetAsyncKeyState(key) & 0x8000;
        }

    private:
        
        const int m_key;
        bool      m_isPressed = false;
        
    };

    class MouseData
    {
    public:
        
        MouseData(HWND hwnd = 0) noexcept
            : m_hwnd(hwnd)
            , m_point()
            , m_beforePoint()
        {}

        POINT GetPoint() const noexcept {
            return m_point;
        }

        POINT GetBeforePoint() const noexcept {
            return m_beforePoint;
        }
        
        POINT GetPosition() noexcept {
            m_beforePoint = m_point;
            return m_point = GetPosition(m_hwnd);
        }

        POINT GetDifference() const noexcept {
            return { m_point.x - m_beforePoint.x, m_point.y - m_beforePoint.y };
        }

        POINT GetCenterPosition() const noexcept {
            WINDOWINFO wi;
            SecureZeroMemory(&wi, sizeof(wi));
            GetWindowInfo(m_hwnd, &wi);
            LONG width = wi.rcClient.right - wi.rcClient.left;
            LONG height = wi.rcClient.bottom - wi.rcClient.top;

            return { wi.rcClient.left + width / 2, wi.rcClient.top + height / 2 };
        }

        POINT GetPositionFromCenter(bool invert_x = false, bool invert_y = false) const noexcept {
            POINT center = GetCenterPosition();
            POINT point = GetPosition(0);

            POINT result = { point.x - center.x, point.y - center.y };
            if (invert_x) {
                result.x *= -1;
            }
            if (invert_y) {
                result.y *= -1;
            }
            return result;
        }

        void LockInCenter() noexcept {
            SetPosition(GetCenterPosition());
        }

        static POINT GetPosition(HWND hwnd) noexcept {
            POINT point;
            GetCursorPos(&point);
            if (hwnd) {
                ScreenToClient(hwnd, &point);
            }
            return point;
        }

        static void SetPosition(POINT point) noexcept {
            SetCursorPos(point.x, point.y);
        }

    private:
        
        const HWND m_hwnd;
        POINT      m_point;
        POINT      m_beforePoint;
        
    };
    
}

#endif
