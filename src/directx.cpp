#include "directx.h"
#include "Application.h"
#include "debug.h"
#include "helper.h"
#include "StanfordParser.h"

Application application;
DebugStream dbg;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    dbg << "Starting directx demo" << std::endl << std::flush;
    std::vector<byte> data = loadBinaryFile("c:/Users/jpc/Development/directx/src/assets/house.ply");
    auto handler = DefaultStanfordHandler{};
    StanfordParser parser(data);
    parser.Parse(handler);
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
