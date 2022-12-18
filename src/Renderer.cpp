#include "Renderer.h"
#include <vector>
#include <numbers>
#include "helper.h"
#include "Mesh.h"

void Renderer::Init(){
    m_registry.Init();
}

void Renderer::InitShaders() {
    auto device = m_deviceResources.GetDevice();
    auto vertexShaderData = loadBinaryFile("CubeVertexShader.cso");
    THROW_IF_FAILED(device->CreateVertexShader(
            vertexShaderData.data(),
            vertexShaderData.size(),
        nullptr, 
        &m_pVertexShader));
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = { {
        .SemanticName = "POSITION",
        .SemanticIndex = 0,
        .Format = DXGI_FORMAT_R32G32B32_FLOAT,
        .InputSlot = 0,
        .AlignedByteOffset = 0,
        .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
        .InstanceDataStepRate = 0,
    }, {
        .SemanticName = "NORMAL",
        .SemanticIndex = 0,
        .Format = DXGI_FORMAT_R32G32B32_FLOAT,
        .InputSlot = 0,
        .AlignedByteOffset = 3 * sizeof(FLOAT),
        .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
        .InstanceDataStepRate = 0,
    }, {
        .SemanticName = "COLOR",
        .SemanticIndex = 0,
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .InputSlot = 0,
        .AlignedByteOffset = 6 * sizeof(FLOAT),
        .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
        .InstanceDataStepRate = 0,
    } };
    THROW_IF_FAILED(device->CreateInputLayout(
        inputElementDesc, 
        ARRAYSIZE(inputElementDesc),
        vertexShaderData.data(),
        vertexShaderData.size(),
        &m_pInputLayout));
    auto pixelShaderData = loadBinaryFile("CubePixelShader.cso");
    THROW_IF_FAILED(device->CreatePixelShader(
        pixelShaderData.data(),
        pixelShaderData.size(),
        nullptr,
        &m_pPixelShader));
}

void Renderer::InitBuffers() {  // a mesh
    auto device = m_deviceResources.GetDevice();

/*
    auto mesh {Mesh::Noise(5, 10, 30)};
    auto vertices = mesh.GetVertices();
*/
/*
    std::vector<byte> data = loadBinaryFile("c:/Users/jpc/Development/directx/src/assets/sphere.ply");
    auto mesh = Mesh::FromStanford(data);
    auto vertices = mesh.GetVertices();
*/
    auto vertices = m_registry.Load();
    m_vertexCount = vertices.size();
    CD3D11_BUFFER_DESC verticesDesc(vertices.size() * sizeof(VertexPositionNormalColor), D3D11_BIND_VERTEX_BUFFER);
    D3D11_SUBRESOURCE_DATA verticesData{
        .pSysMem = vertices.data(),
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0,
    };
    THROW_IF_FAILED(device->CreateBuffer(&verticesDesc, &verticesData, &m_pVertexBuffer));
    
    CD3D11_BUFFER_DESC constantDesc(
        sizeof(ConstantData),
        D3D11_BIND_CONSTANT_BUFFER
    );
    THROW_IF_FAILED(device->CreateBuffer(&constantDesc, nullptr, &m_pConstantBuffer));
}

void Renderer::InitDeviceDependent() {
    InitShaders();
    InitBuffers();
}

void Renderer::InitWindowSizeDependent(){
    auto eye = DirectX::XMVectorSet(-1.0f, 1.0f, 5.0f, 0.f);
    auto at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);
    auto up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);
    DirectX::XMStoreFloat4x4(&m_constantData.view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(eye, at, up)));
    auto viewportSize = m_deviceResources.GetViewportSize();
    float aspectRatioX = static_cast<float>(viewportSize.cx) / static_cast<float>(viewportSize.cy);
    float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;  //?
    float nearPlane = 0.01f;
    float farPlane = 200.0f;
    DirectX::XMStoreFloat4x4(&m_constantData.projection, DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovRH(2.0f * atan(tan(DirectX::XMConvertToRadians(70) * 0.5f) / aspectRatioY), aspectRatioX, nearPlane, farPlane)));
}

void Renderer::Update() {
    m_frameCount++;
    auto angle {static_cast<float>(m_frameCount % 6000) / 3000.0 * std::numbers::pi};
    auto length {2.0f};
    m_center.x = cos(angle) * length;
    m_center.y = sin(angle) * length;
    if (m_frameCount == MAXUINT) {
        m_frameCount = 0;
    }
}

void Renderer::Render() {
    auto deviceContext = m_deviceResources.GetDeviceContext();
    auto renderTargetView = m_deviceResources.GetRenderTargetView();
    auto depthStencilView = m_deviceResources.GetDepthStencilView();

    // clear
    float blue = static_cast<float>(abs(sin(static_cast<double>(m_frameCount) * 3.141592 / 100.0)));
    D3D11_VIDEO_COLOR_RGBA color{ 0.071f, 0.04f, blue, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, reinterpret_cast<float*>(&color));
    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    // Input Assembly stage
    UINT stride = sizeof(VertexPositionNormalColor);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
    deviceContext->IASetInputLayout(m_pInputLayout.Get());
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // Vertex Shader stage
    deviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    deviceContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
    // Pixel Shader stage
    deviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
    // Output Merge stage
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
    // Draw

    auto surfaceExtent {DirectX::XMFLOAT2(m_registry.GetSurfaceExtent(), m_registry.GetSurfaceExtent())};
    auto k {DirectX::XMINT2(floor(m_center.x) - m_registry.GetSurfaceExtent(), floor(m_center.y) - m_registry.GetSurfaceExtent())};
    k = {DirectX::XMINT2(k.x < 0 ? k.x + m_registry.GetPlaygroundEdge() : k.x, k.y < 0 ? k.y + m_registry.GetPlaygroundEdge() : k.y)};
    auto r {DirectX::XMFLOAT2(m_center.x - floor(m_center.x), m_center.y - floor(m_center.y))};

    auto strips = m_registry.GetEntry(RegistryKey::PLAYGROUND);
    for (size_t i = 0; i < 2 * m_registry.GetSurfaceExtent() + 1; i++){
        auto strip = (k.y + i) % m_registry.GetPlaygroundEdge();
        auto entry = strips[strip];
        auto offsetX { - static_cast<float>(k.x) - static_cast<float>(m_registry.GetSurfaceExtent()) - r.x};
        auto offsetY {static_cast<float>(i) - static_cast<float>(m_registry.GetSurfaceExtent()) - r.y};
        DirectX::XMStoreFloat4x4(&m_constantData.world, DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(offsetX, offsetY, 0)));
        deviceContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &m_constantData, 0, 0);
        deviceContext->Draw((m_registry.GetSurfaceExtent() + 1) * 2 * 6, entry.start + static_cast<size_t>(k.x) * 6);
    }
    // set world before uploading constant data
}


