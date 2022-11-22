#include "Application.h"
#include <stdexcept>

void Application::Init(HINSTANCE hInstance, int nCmdShow) {
	WNDCLASSEXW wcex{
		.cbSize = sizeof(WNDCLASSEXW),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = WndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = NULL,
		.hCursor = LoadCursor(nullptr, IDC_ARROW),
		.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH),
		.lpszMenuName = nullptr,
		.lpszClassName = m_windowClassName,
		.hIconSm = NULL,
	};
	if (!RegisterClassExW(&wcex)) {
		throw std::runtime_error("Cannot register window class");
	}
	auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
	auto screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HWND hWnd = CreateWindowW(
		m_windowClassName,
		m_windowTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		(screenWidth - m_windowWidth) / 2,
		(screenHeight - m_windowHeight) / 2,
		m_windowWidth,
		m_windowHeight,
		nullptr,
		nullptr,
		hInstance,
		nullptr);
	if (!hWnd) {
		throw std::runtime_error("Cannot create window");
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	m_deviceResources.Init(hWnd);
	m_renderer.InitDeviceDependent();
	m_renderer.InitWindowSizeDependent();
}

int Application::Run(){
	MSG msg;
	auto done = false;
	while (!done) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// render frame
		m_renderer.Update();
		m_renderer.Render();
		m_deviceResources.Present();
		done = msg.message == WM_QUIT;
	}
	return (int)msg.wParam;
}

void Application::Done(HINSTANCE hInstance) {
	UnregisterClassW(m_windowClassName, hInstance);
}

LRESULT Application::InstanceWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return DefWindowProcW(hWnd, message, wParam, lParam);
}
