#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_IMGUI_IMGUIHELPER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_IMGUI_IMGUIHELPER_H_

#include <Windows.h>

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui_internal.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "misc/cpp/imgui_stdlib.h"

namespace imgui_helper
{
    inline void Init(HWND hWnd, ID3D11Device* dev, ID3D11DeviceContext* ctx) {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Setup Dear ImGui Style
        ImGui::StyleColorsClassic();

        // Setup Platform/Renderer bindings
        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(dev, ctx);

#include "3rd/ja_glyph_ranges.h"
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig config;
        config.MergeMode = true;
        io.Fonts->AddFontDefault();
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msgothic.ttc", 13.f, &config, glyphRangesJapanese);
    }

    inline void Begin() {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    inline void Draw() {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    inline void Release() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

};

#endif
