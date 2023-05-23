#include "Application.h"
#include <stdexcept>

Application::Application(HINSTANCE hInst, int nCmdShow): hInstance(hInst) {
/*
    WNDCLASSEXW wcex {
            sizeof(WNDCLASSEXW),
            CS_HREDRAW | CS_VREDRAW,
            WndProc,
            0,
            0,
            hInstance,
            nullptr,
            LoadCursor(nullptr, IDC_ARROW),
            static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)),
            nullptr,
            m_windowClassName,
            nullptr,
    };
    if (!RegisterClassExW(&wcex)) throw std::runtime_error("Cannot register window class");
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
    if (!hWnd) throw std::runtime_error("Cannot create window");
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
*/
    m_pApplicationClass = std::make_unique<ApplicationClass>(hInst);
    auto& w {m_pApplicationClass->createWindow()};
    w.show(nCmdShow);
    w.update();
    m_pDeviceResources = std::make_unique<DeviceResources>(w.getHandle());
    m_pRenderer = std::make_unique<Renderer>(m_pDeviceResources);
}

Application::Application(Application &&a) noexcept {
    hInstance = a.hInstance;
    a.hInstance = nullptr;
}

Application &Application::operator=(Application &&a) noexcept {
    hInstance = a.hInstance;
    a.hInstance = nullptr;
    return *this;
}

Application::~Application() {
    if (hInstance) UnregisterClassW(m_windowClassName, hInstance);
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
		m_pRenderer->Update();
		m_pRenderer->Render();
		m_pDeviceResources->Present();
		done = msg.message == WM_QUIT;
	}
	return static_cast<int>(msg.wParam);
}

LRESULT Application::InstanceWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return DefWindowProcW(hWnd, message, wParam, lParam);
}


