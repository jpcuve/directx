#include "directx.h"
#include "Application.h"
#include "debug.h"
#include "helper.h"

Application application;
DebugStream dbg;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    dbg << "Starting dirctx demo" << std::endl;
    for (int i = -10; i < 10; i++){
        dbg << i << " " << PositiveModulo(i, 3) << std::endl;
    }
    dbg << std::flush;
    application.Init(hInstance, nCmdShow);
    int retVal = application.Run();
    application.Done(hInstance);
    return retVal;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return application.InstanceWndProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
