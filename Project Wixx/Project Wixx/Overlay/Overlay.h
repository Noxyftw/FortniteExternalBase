#pragma once

#include <Windows.h>
#include <string>
#include <chrono>

#include <D3D11.h>
#include "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\\D3DX11core.h"
#include <D3DX11.h>
#include "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include\\D3DX11tex.h"
#include <d3d9types.h>
#include <Uxtheme.h>
#include <dwmapi.h>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_internal.h"

#include "../Settings/lazy.hxx"
#include "../Overlay/UI/GUI.h"
#include "../Actor/Actorloop.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dwmapi.lib")

HWND hwnd;
RECT rc;

ID3D11Device* d3d_device = nullptr;
ID3D11DeviceContext* d3d_device_ctx = nullptr;
IDXGISwapChain* d3d_swap_chain = nullptr;
ID3D11RenderTargetView* d3d_render_target = nullptr;
ID3D11RasterizerState* d3d_rasterizer_state = nullptr;

ImFont* font = nullptr;

// Noxyftw

static constexpr const char* OverlayClass = "ProjectJizzOverlayClass";
static constexpr const char* OverlayTitle = "Project Jizz Overlay";

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK OverlayWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (d3d_device != nullptr && d3d_swap_chain != nullptr && wParam != SIZE_MINIMIZED)
        {
            if (d3d_render_target)
            {
                d3d_render_target->Release();
                d3d_render_target = nullptr;
            }

            d3d_swap_chain->ResizeBuffers(0, static_cast<UINT>(LOWORD(lParam)), static_cast<UINT>(HIWORD(lParam)), DXGI_FORMAT_UNKNOWN, 0);

            ID3D11Texture2D* pBackBuffer = nullptr;
            HRESULT hr = d3d_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

            if (SUCCEEDED(hr) && pBackBuffer)
            {
                d3d_device->CreateRenderTargetView(pBackBuffer, nullptr, &d3d_render_target);
                pBackBuffer->Release();
            }
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

namespace overlay
{
    bool create_window()
    {
        int screen_w = GetSystemMetrics(SM_CXSCREEN);
        int screen_h = GetSystemMetrics(SM_CYSCREEN);

        WNDCLASSEXA wc = {};
        wc.cbSize = sizeof(WNDCLASSEXA);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = OverlayWndProc;
        wc.hInstance = GetModuleHandleA(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        wc.lpszClassName = OverlayClass;

        if (!RegisterClassExA(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
            return false;

        globals.window_handle = CreateWindowExA(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_NOACTIVATE, OverlayClass, OverlayTitle, WS_POPUP, 0, 0, screen_w, screen_h, nullptr, nullptr, GetModuleHandleA(nullptr), nullptr);

        if (!globals.window_handle)
            return false;

        SetLayeredWindowAttributes(globals.window_handle, RGB(0, 0, 0), 255, LWA_ALPHA);

        MARGINS margin = { -1, -1, -1, -1 };
        DwmExtendFrameIntoClientArea(globals.window_handle, &margin);

        return true;
    }

    bool init()
    {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = globals.window_handle;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL feature_lvl;
        const D3D_FEATURE_LEVEL feature_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

        HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, feature_array, 2, D3D11_SDK_VERSION, &sd, &d3d_swap_chain, &d3d_device, &feature_lvl, &d3d_device_ctx);

        if (FAILED(hr))
            return false;

        ID3D11Texture2D* pBackBuffer = nullptr;
        hr = d3d_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        if (FAILED(hr) || !pBackBuffer)
            return false;

        hr = d3d_device->CreateRenderTargetView(pBackBuffer, nullptr, &d3d_render_target);
        pBackBuffer->Release();

        if (FAILED(hr))
            return false;

        D3D11_RASTERIZER_DESC rasterizer_desc = {};
        rasterizer_desc.FillMode = D3D11_FILL_SOLID;
        rasterizer_desc.CullMode = D3D11_CULL_NONE;
        rasterizer_desc.MultisampleEnable = TRUE;
        rasterizer_desc.AntialiasedLineEnable = TRUE;
        d3d_device->CreateRasterizerState(&rasterizer_desc, &d3d_rasterizer_state);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        font = io.Fonts->AddFontDefault();

        ImGui_ImplWin32_Init(globals.window_handle);
        ImGui_ImplDX11_Init(d3d_device, d3d_device_ctx);

        return true;
    }

    void menu_loop()
    {
        static bool key_was_pressed = false;

        bool key_is_pressed = ((GetAsyncKeyState(Visuals::menukey) & 0x8000) != 0) || ((GetAsyncKeyState(VK_INSERT) & 0x8000) != 0);

        if (key_is_pressed && !key_was_pressed)
            globals.show_menu = !globals.show_menu;

        key_was_pressed = key_is_pressed;

        if (globals.show_menu)
            menu::render();
    }

    void update_input()
    {
        ImGuiIO& io = ImGui::GetIO();

        POINT p_cursor = {};
        GetCursorPos(&p_cursor);
        io.MousePos.x = static_cast<float>(p_cursor.x);
        io.MousePos.y = static_cast<float>(p_cursor.y);

        static bool mouse_was_down = false;
        bool mouse_is_down = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

        if (mouse_is_down && !mouse_was_down)
        {
            io.MouseDown[0] = true;
            io.MouseClicked[0] = true;
            io.MouseClickedPos[0] = io.MousePos;
        }
        else if (!mouse_is_down)
        {
            io.MouseDown[0] = false;
        }

        mouse_was_down = mouse_is_down;
    }

    void draw()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        Game();
        menu_loop();

        ImGui::Render();

        if (d3d_rasterizer_state)
            d3d_device_ctx->RSSetState(d3d_rasterizer_state);

        const float clear[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

        d3d_device_ctx->OMSetRenderTargets(1, &d3d_render_target, nullptr);
        d3d_device_ctx->ClearRenderTargetView(d3d_render_target, clear);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        d3d_swap_chain->Present(0, 0);
    }

    bool render()
    {
        MSG msg = {};
        ZeroMemory(&msg, sizeof(MSG));

        static auto last_time = std::chrono::high_resolution_clock::now();

        while (msg.message != WM_QUIT)
        {
            if (PeekMessageA(&msg, globals.window_handle, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> delta_time = current_time - last_time;
            last_time = current_time;

            ImGuiIO& io = ImGui::GetIO();
            io.DeltaTime = delta_time.count();

            update_input();
            draw();
        }

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        if (d3d_rasterizer_state)
        {
            d3d_rasterizer_state->Release();
            d3d_rasterizer_state = nullptr;
        }

        if (d3d_render_target)
        {
            d3d_render_target->Release();
            d3d_render_target = nullptr;
        }

        if (d3d_swap_chain)
        {
            d3d_swap_chain->Release();
            d3d_swap_chain = nullptr;
        }

        if (d3d_device_ctx)
        {
            d3d_device_ctx->Release();
            d3d_device_ctx = nullptr;
        }

        if (d3d_device)
        {
            d3d_device->Release();
            d3d_device = nullptr;
        }

        if (globals.window_handle && IsWindow(globals.window_handle))
        {
            DestroyWindow(globals.window_handle);
            globals.window_handle = nullptr;
        }

        UnregisterClassA(OverlayClass, GetModuleHandleA(nullptr));

        return true;
    }

    void start()
    {
        if (!create_window())
            return;

        if (!init())
            return;

        ShowWindow(globals.window_handle, SW_SHOW);
        UpdateWindow(globals.window_handle);

        render();
    }
}