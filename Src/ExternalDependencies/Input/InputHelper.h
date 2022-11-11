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
            Position(const Position& pos) noexcept
                : POINT(pos)
            {}
            constexpr Position(LONG x, LONG y) noexcept
                : POINT({ x, y })
            {}

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
            // Compound assignment
            Position& operator+=(const Position& pos) noexcept {
                x += pos.x;
                y += pos.y;
                return *this;
            }
            Position& operator-=(const Position& pos) noexcept {
                x -= pos.x;
                y -= pos.y;
                return *this;
            }
            Position& operator*=(const Position& pos) noexcept {
                x *= pos.x;
                y *= pos.y;
                return *this;
            }
            Position& operator/=(const Position& pos) noexcept {
                x /= pos.x;
                y /= pos.y;
                return *this;
            }
        };
        // Arithmetic
        friend Position operator+(const Position& pos1, const Position& pos2) noexcept {
            return Position(pos1) += pos2;
        }
        friend Position operator-(const Position& pos1, const Position& pos2) noexcept {
            return Position(pos1) -= pos2;
        }
        friend Position operator*(const Position& pos1, const Position& pos2) noexcept {
            return Position(pos1) *= pos2;
        }
        friend Position operator/(const Position& pos1, const Position& pos2) noexcept {
            return Position(pos1) /= pos2;
        }
        
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

        Position GetPositionFromCenter() const noexcept {
            return Position(GetPosition(0)) - GetCenterPosition();
        }
        Position GetPositionFromCenter(const Position& limit) const noexcept {
            Position relative = GetPositionFromCenter();
            if (relative.x > limit.x) {
                relative.x = limit.x;
            }
            else if (relative.x < -limit.x) {
                relative.x = -limit.x;
            }
            if (relative.y > limit.y) {
                relative.y = limit.y;
            }
            else if (relative.y < -limit.y) {
                relative.y = -limit.y;
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

        static void ShowCursor(bool show) noexcept {
            CURSORINFO ci{};
            SecureZeroMemory(&ci, sizeof(ci));
            ci.cbSize = sizeof(ci);
            GetCursorInfo(&ci);
            if (show) {
                // When the cursor is hidden
                if (!ci.flags) {
                    ::ShowCursor(TRUE);
                }
            }
            else {
                // When the cursor is showing
                if (ci.flags || CURSOR_SHOWING) {
                    ::ShowCursor(FALSE);
                }
            }
        }

        Position   m_position;
        Position   m_beforePosition;

    private:
        
        const HWND m_hwnd;
        
    };
    
}

#endif
