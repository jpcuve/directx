#pragma once
#include "stdafx.h"
#include "DeviceResources.h"
#include "Renderer.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class Application {
private:
	LPCWSTR m_windowClassName {L"DirectXWindowClassName"};
	LPCWSTR m_windowTitle {L"DirectX"};
	int m_windowWidth {1280};
	int m_windowHeight {1024};
	DeviceResources m_deviceResources;
	Renderer m_renderer{ m_deviceResources };
public:
	void Init(HINSTANCE hInst, int nCmdShow);
	int Run();
	void Done(HINSTANCE hInst);
	LRESULT CALLBACK InstanceWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

