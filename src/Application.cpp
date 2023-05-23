#include "Application.h"

Application::Application(HINSTANCE hInst, int nCmdShow){
    m_pApplicationClass = std::make_unique<ApplicationClass>(hInst);
    auto& w {m_pApplicationClass->createWindow()};
    w.show(nCmdShow);
    w.update();
    m_pDeviceResources = std::make_unique<DeviceResources>(w.getHandle());
    m_pRenderer = std::make_unique<Renderer>(m_pDeviceResources);
}

WPARAM Application::Run(){
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
	return msg.wParam;
}


