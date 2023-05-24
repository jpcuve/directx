//
// Created by jpcuv on 14-05-23.
//

#include "MainWindow.h"
#include <stdexcept>

MainWindow::MainWindow(HINSTANCE hInstance, LPCWSTR windowClassName) {
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
    m_pDeviceResources = std::make_unique<DeviceResources>(m_hwnd);
    m_pRenderer = std::make_unique<Renderer>(*m_pDeviceResources);
}

LRESULT MainWindow::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(m_hwnd, message, wParam, lParam);
}

void MainWindow::RenderFrame() {
    m_pRenderer->Update();
    m_pRenderer->Render();
    m_pDeviceResources->Present();
}
