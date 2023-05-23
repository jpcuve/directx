//
// Created by jpcuv on 14-05-23.
//

#include "ApplicationWindow.h"
#include <stdexcept>

ApplicationWindow::ApplicationWindow(HINSTANCE hInstance, LPCWSTR windowClassName) {
    auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
    auto screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int width = screenWidth / 2;
    int height = screenHeight / 2;
    m_hwnd = CreateWindowW(
            windowClassName,
            L"DirectX",
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
            (screenWidth - width) / 2,
            (screenHeight - height) / 2,
            width,
            height,
            nullptr,
            nullptr,
            hInstance,
            nullptr);
    if (!m_hwnd) throw std::runtime_error("Cannot create window");
}

LRESULT ApplicationWindow::wndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(m_hwnd, message, wParam, lParam);
}
