#pragma once

#include <memory>
#include "stdafx.h"
#include "DeviceResources.h"
#include "Renderer.h"
#include "ApplicationClass.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class Application {
private:
    HINSTANCE hInstance;
	LPCWSTR m_windowClassName {L"DirectXWindowClassName"};
	LPCWSTR m_windowTitle {L"DirectX"};
	int m_windowWidth {768};
	int m_windowHeight {768};
    std::unique_ptr<ApplicationClass> m_pApplicationClass;
    std::unique_ptr<DeviceResources> m_pDeviceResources;
    std::unique_ptr<Renderer> m_pRenderer;
	// DeviceResources m_deviceResources;
	// Renderer m_renderer{ m_deviceResources };
public:
    Application(HINSTANCE hInst, int nCmdShow);
    Application(const Application& instance) = delete;
    Application& operator=(const Application& instance) = delete;
    Application(Application &&a) noexcept ;
    Application &operator=(Application &&a) noexcept ;
    ~Application();
	int Run();
	LRESULT CALLBACK InstanceWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

