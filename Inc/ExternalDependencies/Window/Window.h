#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_WINDOW_WINDOW_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_WINDOW_WINDOW_H_

#include <cstdint>
#include <string_view>

#include <Windows.h>

#include "Utility/Assert.h"

#include "strconv.h"

class Window
{
public:

    struct Size {

        Size() : x(0), y(0) {}
        Size(int32_t x, int32_t y) : x(x), y(y) {}

        union {
            struct {
                int32_t x;
                int32_t y;
            };
            struct {
                int32_t w;
                int32_t h;
            };
        };
    };

    ~Window() {
        Release();
    }

    bool Create(std::string_view title, const Size& size) {
        m_size = size;
        HINSTANCE hInstance = GetModuleHandle(nullptr);
        std::wstring class_name = sjis_to_wide(title.data());
        std::wstring window_name = sjis_to_wide(title.data());

        WNDCLASSEX wcex{};
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = 0;
        wcex.lpfnWndProc = Window::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = nullptr;
        wcex.hCursor = nullptr;
        wcex.hbrBackground = nullptr;
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = class_name.c_str();

        if (!RegisterClassEx(&wcex)) {
            assert::ShowError(ASSERT_FILE_LINE, "RegisterClassEx failed.");
            return false;
        }

        m_hwnd = CreateWindowEx(
            0,
            class_name.c_str(),
            window_name.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            m_size.x,
            m_size.y,
            nullptr,
            nullptr,
            hInstance,
            this
        );

        if (!m_hwnd) {
            assert::ShowError(ASSERT_FILE_LINE, "CreateWindowEx failed.");
            return false;
        }

        Resize(size);

        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);

        return true;
    }

    bool ProcessMessage() {
        MSG msg{};
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }

    void Resize(const Size& size) {
        m_size = size;
        WINDOWINFO wi{};
        SecureZeroMemory(&wi, sizeof(wi));
        GetWindowInfo(m_hwnd, &wi);
        LONG new_width = (wi.rcWindow.right - wi.rcWindow.left) - (wi.rcClient.right - wi.rcClient.left) + m_size.w;
        LONG new_height = (wi.rcWindow.bottom - wi.rcWindow.top) - (wi.rcClient.bottom - wi.rcClient.top) + m_size.h;
        SetWindowPos(m_hwnd, NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
    }

private:

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
        if (msg == WM_NCCREATE) {
            LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lparam);
            Window* window = reinterpret_cast<Window*>(cs->lpCreateParams);
            // Extract the address of the 'Window' instance and store it in the GWLP_USERDATA.
            // From this point on, this address can be retrieved given the window handle.
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        }
        // Retrieve the Window instance based on the window handle.
        // This can only be done successfully after the GWLP_USERDATA has been set.
        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (window) {
            return window->WndProcImpl(hwnd, msg, wparam, lparam);
        }
        // The GWLP_USERDATA property was not found, so call the default window procedure.
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    LRESULT CALLBACK WndProcImpl(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
        switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return 0;
    }

    void Release() {
        if (m_hwnd) {
            DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }
    }

    HWND m_hwnd = nullptr;
    Size m_size;

};

#endif
