#include "directx.h"

#include <memory>
#include "Application.h"
#include "debug.h"
#include "helper.h"

DebugStream dbg;

std::unique_ptr<Application> app;

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
    app = std::make_unique<Application>(hInstance, nCmdShow);
    return app->Run();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return app->InstanceWndProc(hWnd, message, wParam, lParam);
}
