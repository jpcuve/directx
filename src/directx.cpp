#include "directx.h"
#include "Application.h"
#include "debug.h"
#include "GameModel.h"

Application application;
DebugStream dbg;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    dbg << "Starting directx demo" << std::endl << std::flush;
    GameModel gameModel;
    for (int i = 0; i < 60; i++){
        auto position {DirectX::XMFLOAT2{static_cast<float>(i), 0}};
        dbg << gameModel.GetHeight(position) << std::endl;
    }
    auto position {DirectX::XMFLOAT2{43, 19}};
    auto map {gameModel.LocalMap(position)};
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
