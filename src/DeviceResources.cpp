#include "DeviceResources.h"
#include "helper.h"

void DeviceResources::InitDeviceResources() {
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_9_1,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_11_1
    };
    UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    THROW_IF_FAILED(D3D11CreateDevice(
        nullptr,                    // Specify nullptr to use the default adapter.
        D3D_DRIVER_TYPE_HARDWARE,   // Create a device using the hardware graphics driver.
        0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
        deviceFlags,                // Set debug and Direct2D compatibility flags.
        levels,                     // List of feature levels this app can support.
        ARRAYSIZE(levels),          // Size of the list above.
        D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
        &m_pDevice,                    // Returns the Direct3D device created.
        &m_featureLevel,            // Returns feature level of device created.
        &m_pDeviceContext                    // Returns the device immediate context.
    ));
}

void DeviceResources::InitWindowResources(HWND hWnd) {
    RECT rect;
    GetWindowRect(hWnd, &rect);
    auto width = rect.right - rect.left;
    auto height = rect.bottom - rect.top;
    // TODO make sure this really will be a flip model
    DXGI_SWAP_CHAIN_DESC sd{
        .BufferDesc {
            .Width = static_cast<UINT>(width),
            .Height = static_cast<UINT>(height),
            .RefreshRate{
                .Numerator = 60,
                .Denominator = 1,
            },
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        },
        .SampleDesc {  // anti-aliasing setting
            .Count = 1,
            .Quality = 0,  // vendor specific
        },
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 2,
        .OutputWindow = hWnd,
        .Windowed = TRUE,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
    };
    ComPtr<IDXGIDevice3> dxgiDevice;
    m_pDevice.As(&dxgiDevice);
    ComPtr<IDXGIAdapter> adapter;
    ComPtr<IDXGIFactory> factory;
    THROW_IF_FAILED(dxgiDevice->GetAdapter(&adapter));
    THROW_IF_FAILED(adapter->GetParent(IID_PPV_ARGS(&factory)));
    THROW_IF_FAILED(factory->CreateSwapChain(m_pDevice.Get(), &sd, &m_pSwapChain));
    SetUp();
}

void DeviceResources::SetUp() {
    THROW_IF_FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &m_pBackBuffer));
    THROW_IF_FAILED(m_pDevice->CreateRenderTargetView(m_pBackBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf()));
    D3D11_TEXTURE2D_DESC backBufferDesc;
    m_pBackBuffer->GetDesc(&backBufferDesc);
    m_viewportSize = {
        .cx = static_cast<LONG>(backBufferDesc.Width),
        .cy = static_cast<LONG>(backBufferDesc.Height)
    };
    D3D11_TEXTURE2D_DESC depthStencilDesc{
        .Width = backBufferDesc.Width,
        .Height = backBufferDesc.Height,
        .MipLevels = 1,
        .ArraySize = 1,
        .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
        .SampleDesc {  // same as swap chain
            .Count = 1,
            .Quality = 0,
        },
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_DEPTH_STENCIL,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
    };
    THROW_IF_FAILED(m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencil));
    THROW_IF_FAILED(m_pDevice->CreateDepthStencilView(m_pDepthStencil.Get(), nullptr, &m_pDepthStencilView));
    D3D11_VIEWPORT vp{
        .TopLeftX = 0.0f,
        .TopLeftY = 0.0f,
        .Width = static_cast<float>(backBufferDesc.Width),
        .Height = static_cast<float>(backBufferDesc.Height),
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f,
    };
    m_pDeviceContext->RSSetViewports(1, &vp);
    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), nullptr);
}

void DeviceResources::TearDown() {
    m_pRenderTargetView.Reset();
    m_pBackBuffer.Reset();
    m_pDepthStencilView.Reset();
    m_pDepthStencil.Reset();
    m_pDeviceContext->Flush();
}

void DeviceResources::Init(HWND hWnd) {
    InitDeviceResources();
    InitWindowResources(hWnd);
}

void DeviceResources::Present() {
    THROW_IF_FAILED(m_pSwapChain->Present(1, 0));
}
