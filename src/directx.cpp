#include "directx.h"

#include <memory>
#include "MainWindowClass.h"
#include "DeviceResources.h"
#include "Renderer.h"
#include "debug.h"
#include "helper.h"

DebugStream dbg;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    dbg << "Starting directx demo" << std::endl;
    for (int i = -10; i < 10; i++) {
        dbg << i << " " << PositiveModulo(i, 3) << std::endl;
    }
    dbg << std::flush;
    MainWindowClass applicationClass {hInstance};
    auto& w {applicationClass.createWindow()};
    w.show(nCmdShow);
    w.update();
    DeviceResources deviceResources{w.getHandle()};
    Renderer renderer {deviceResources};
//    auto pDeviceResources = std::make_unique<DeviceResources>(w.getHandle());
    // auto pRenderer = std::make_unique<Renderer>(pDeviceResources);
    MSG msg;
    auto done = false;
    while (!done) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // render frame
        renderer.Update();
        renderer.Render();
        deviceResources.Present();
        done = msg.message == WM_QUIT;
    }
    return static_cast<int>(msg.wParam);
}
