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

    class CursorData
    {
    public:

        struct Position : public POINT {
            Position() noexcept
                : POINT({ 0, 0 })
            {}
            Position(const POINT& pos) noexcept
                : POINT(pos)
            {}
            constexpr Position(LONG x, LONG y) noexcept
                : POINT({ x, y })
            {}
            Position(const Position&) = default;
            Position(Position&&)      = default;

            // Assignment
            Position& operator=(const POINT& pos) noexcept {
                x = pos.x;
                y = pos.y;
                return *this;
            }
            Position& operator=(const Position& pos) noexcept {
                x = pos.x;
                y = pos.y;
                return *this;
            }
            Position& operator=(Position&& pos) noexcept {
                x = pos.x;
                y = pos.y;
                return *this;
            }
            Position operator+(const Position& pos) const noexcept {
                return Position(x + pos.x, y + pos.y);
            }
            Position operator-(const Position& pos) const noexcept {
                return Position(x - pos.x, y - pos.y);
            }
        };
        
        CursorData(HWND hwnd = 0) noexcept
            : m_hwnd(hwnd)
            , m_position()
            , m_beforePosition()
        {}
        
        Position GetPosition() noexcept {
            m_beforePosition = m_position;
            return m_position = GetPosition(m_hwnd);
        }

        Position GetDifference() const noexcept {
            return m_beforePosition - m_position;
        }

        Position GetCenterPosition() const noexcept {
            WINDOWINFO wi{};
            SecureZeroMemory(&wi, sizeof(wi));
            GetWindowInfo(m_hwnd, &wi);
            LONG width  = wi.rcClient.right  - wi.rcClient.left;
            LONG height = wi.rcClient.bottom - wi.rcClient.top;
            return { wi.rcClient.left + width / 2, wi.rcClient.top + height / 2 };
        }

        Position GetPositionFromCenter(bool invert_x = false, bool invert_y = false) const noexcept {
            Position relative = Position(GetPosition(0)) - GetCenterPosition();
            if (invert_x) {
                relative.x *= -1;
            }
            if (invert_y) {
                relative.y *= -1;
            }
            return relative;
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

        Position   m_position;
        Position   m_beforePosition;

    private:
        
        const HWND m_hwnd;

    };
    
}

#endif
