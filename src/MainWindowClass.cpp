//
// Created by jpcuv on 14-05-23.
//

#include "MainWindowClass.h"
#include <stdexcept>

std::vector<MainWindow> MainWindowClass::m_windows {};

MainWindowClass::MainWindowClass(HINSTANCE hInstance): m_hInstance(hInstance) {
    WNDCLASSEXW wcex {
            sizeof(WNDCLASSEXW),
            CS_HREDRAW | CS_VREDRAW,
            ApplicationWndProc,
            0,
            0,
            hInstance,
            nullptr,
            LoadCursor(nullptr, IDC_ARROW),
            static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)),
            nullptr,
            m_pClassName,
            nullptr,
    };
    if (!RegisterClassExW(&wcex)) throw std::runtime_error("Cannot register window class");
}

MainWindowClass::~MainWindowClass() {
    if (m_hInstance) UnregisterClassW(m_pClassName, m_hInstance);
}

LRESULT MainWindowClass::ApplicationWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    for (auto& window: m_windows){
        if (window.getHandle() == hWnd){
            return window.wndProc(message, wParam, lParam);
        }
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

MainWindow &MainWindowClass::createWindow() {
    m_windows.emplace_back(m_hInstance, m_pClassName);
    return m_windows.back();
}
