#pragma once

#include <Windows.h>

namespace input_helper {

    class KeyData
    {
    public:

        constexpr KeyData(int key) noexcept
            : key(key)
        {}

        int GetKey() const noexcept {
            return key;
        }

        bool GetState(bool is_press_and_hold = false) {
            if (is_press_and_hold) {
                if (isPressed) {
                    return false;
                }
            }
            isPressed = GetState(key);
            return isPressed;
        }

        static bool GetState(int key) noexcept {
            return GetAsyncKeyState(key) & 0x8000;
        }

    private:
        
        const int key;
        bool      isPressed = false;
        
    };

    class MouseData
    {
    public:
        
        MouseData(HWND hwnd = 0) noexcept
            : hwnd(hwnd)
            , point()
            , beforePoint()
        {}

        POINT GetPoint() const noexcept {
            return point;
        }

        POINT GetBeforePoint() const noexcept {
            return beforePoint;
        }
        
        POINT GetPosition() noexcept {
            beforePoint = point;
            point = GetPosition(hwnd);
            return point;
        }

        POINT GetDifference() noexcept {
            return { point.x - beforePoint.x, point.y - beforePoint.y };
        }

        static POINT GetPosition(HWND hwnd) {
            POINT point;
            GetCursorPos(&point);
            if (hwnd) {
                ScreenToClient(hwnd, &point);
            }
            return point;
        }

    private:
        
        const HWND hwnd;
        POINT      point;
        POINT      beforePoint;
        
    };
    
}
