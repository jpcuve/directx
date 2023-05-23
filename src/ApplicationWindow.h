//
// Created by jpcuv on 14-05-23.
//

#ifndef DIRECTX_APPLICATIONWINDOW_H
#define DIRECTX_APPLICATIONWINDOW_H

#include <map>

class ApplicationWindow {
private:
    HWND m_hwnd;
public:
    ApplicationWindow(HINSTANCE hInstance, LPCWSTR windowClassName);
    ~ApplicationWindow();
    ApplicationWindow(const ApplicationWindow &that) = delete;
    ApplicationWindow(ApplicationWindow &&that) noexcept ;
    ApplicationWindow &operator=(const ApplicationWindow &that) = delete;
    ApplicationWindow &operator=(ApplicationWindow &&that) noexcept;
    HWND getHandle() { return m_hwnd; };
    LRESULT wndProc(UINT message, WPARAM wParam, LPARAM lParam);
    inline BOOL show(int nCmdShow){ return ShowWindow(m_hwnd, nCmdShow); }
    inline BOOL update(){ return UpdateWindow(m_hwnd); }
};


#endif //DIRECTX_APPLICATIONWINDOW_H
