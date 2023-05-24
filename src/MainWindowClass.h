//
// Created by jpcuv on 14-05-23.
//

#ifndef DIRECTX_MAINWINDOWCLASS_H
#define DIRECTX_MAINWINDOWCLASS_H

#include <vector>
#include "MainWindow.h"

class MainWindowClass {
private:
    HINSTANCE m_hInstance;
    LPCWSTR m_pClassName {L"Application"};
    static std::vector<MainWindow> m_windows;
    static LRESULT ApplicationWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    explicit MainWindowClass(HINSTANCE hInstance);
    ~MainWindowClass();
    MainWindowClass(const MainWindowClass &that) = delete;
    MainWindowClass &operator=(const MainWindowClass &that) = delete;
    MainWindow &createWindow();
};


#endif //DIRECTX_MAINWINDOWCLASS_H
