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

ApplicationWindow::~ApplicationWindow() {
    if (m_hwnd){
        // do not do anything for a moved application window
    }
}

ApplicationWindow::ApplicationWindow(ApplicationWindow &&that) noexcept {
    m_hwnd = that.m_hwnd;
    that.m_hwnd = nullptr;
}

ApplicationWindow &ApplicationWindow::operator=(ApplicationWindow &&that)  noexcept {
    m_hwnd = that.m_hwnd;
    that.m_hwnd = nullptr;
    return *this;
}

LRESULT ApplicationWindow::wndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(m_hwnd, message, wParam, lParam);
}
