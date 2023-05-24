//
// Created by jpcuv on 14-05-23.
//

#ifndef DIRECTX_MAINWINDOW_H
#define DIRECTX_MAINWINDOW_H

#include <map>
#include "DeviceResources.h"
#include "Renderer.h"

class MainWindow {
private:
    HWND m_hwnd;
    std::unique_ptr<DeviceResources> m_pDeviceResources;
    std::unique_ptr<Renderer> m_pRenderer;
public:
    MainWindow(HINSTANCE hInstance, LPCWSTR windowClassName);
    HWND getHandle() { return m_hwnd; };
    LRESULT wndProc(UINT message, WPARAM wParam, LPARAM lParam);
    inline BOOL show(int nCmdShow){ return ShowWindow(m_hwnd, nCmdShow); }
    inline BOOL update(){ return UpdateWindow(m_hwnd); }
    void RenderFrame();
};


#endif //DIRECTX_MAINWINDOW_H
