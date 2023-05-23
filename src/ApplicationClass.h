//
// Created by jpcuv on 14-05-23.
//

#ifndef DIRECTX_APPLICATIONCLASS_H
#define DIRECTX_APPLICATIONCLASS_H

#include <vector>
#include "ApplicationWindow.h"

class ApplicationClass {
private:
    HINSTANCE m_hInstance;
    LPCWSTR m_pClassName {L"Application"};
    static std::vector<ApplicationWindow> m_windows;
    static LRESULT ApplicationWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    explicit ApplicationClass(HINSTANCE hInstance);
    ~ApplicationClass();
    ApplicationClass(const ApplicationClass &that) = delete;
    ApplicationClass &operator=(const ApplicationClass &that) = delete;
    ApplicationWindow &createWindow();
};


#endif //DIRECTX_APPLICATIONCLASS_H
