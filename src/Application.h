#pragma once
#include "stdafx.h"
#include "DeviceResources.h"
#include "Renderer.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class Application {
private:
    HINSTANCE hInstance;
	LPCWSTR m_windowClassName {L"DirectXWindowClassName"};
	LPCWSTR m_windowTitle {L"DirectX"};
	int m_windowWidth {768};
	int m_windowHeight {768};
	DeviceResources m_deviceResources;
	Renderer m_renderer{ m_deviceResources };
public:
    Application(HINSTANCE hInst, int nCmdShow);
    ~Application();
	int Run();
	LRESULT CALLBACK InstanceWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

