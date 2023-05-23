#pragma once

#include <memory>
#include "stdafx.h"
#include "DeviceResources.h"
#include "Renderer.h"
#include "ApplicationClass.h"

class Application {
private:
    std::unique_ptr<ApplicationClass> m_pApplicationClass;
    std::unique_ptr<DeviceResources> m_pDeviceResources;
    std::unique_ptr<Renderer> m_pRenderer;
public:
    Application(HINSTANCE hInst, int nCmdShow);
	WPARAM Run();
};

