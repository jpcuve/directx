#include "Renderer.h"
#include <vector>
#include "helper.h"
#include "Mesh.h"

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
        .SemanticName = "COLOR",
        .SemanticIndex = 0,
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .InputSlot = 0,
        .AlignedByteOffset = 3 * sizeof(FLOAT),
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

void Renderer::InitBuffers() {  // a cube
    auto device = m_deviceResources.GetDevice();

    VertexPositionColor cubeVertices[] = {
        {DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f), {0x00, 0x00, 0x00, 0xFF}},
        {DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f), {0x00, 0x00, 0xFF, 0xFF}},
        {DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f), {0x00, 0xFF, 0x00, 0xFF}},
        {DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), {0x00, 0xFF, 0xFF, 0xFF}},

        {DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f), {0xFF, 0x00, 0x00, 0xFF}},
        {DirectX::XMFLOAT3(0.5f,-0.5f, 0.5f), {0xFF, 0x00, 0xFF, 0xFF}},
        {DirectX::XMFLOAT3(0.5f, 0.5f,-0.5f), {0xFF, 0xFF, 0x00, 0xFF}},
        {DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), {0xFF, 0xFF, 0xFF, 0xFF}},
    };
    CD3D11_BUFFER_DESC verticesDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
    D3D11_SUBRESOURCE_DATA verticesData{
        .pSysMem = &cubeVertices,
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0,
    };
    THROW_IF_FAILED(device->CreateBuffer(&verticesDesc, &verticesData, &m_pVertexBuffer));
    
    unsigned short cubeIndices[] = {
        0,2,1, // -x
        1,2,3,

        4,5,6, // +x
        5,7,6,

        0,1,5, // -y
        0,5,4,

        2,6,7, // +y
        2,7,3,

        0,4,6, // -z
        0,6,2,

        1,3,7, // +z
        1,7,5,
    };
    m_indexCount = ARRAYSIZE(cubeIndices);
    CD3D11_BUFFER_DESC indicesDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
    D3D11_SUBRESOURCE_DATA indicesData{
        .pSysMem = &cubeIndices,
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0
    };
    THROW_IF_FAILED(device->CreateBuffer(&indicesDesc, &indicesData, &m_pIndexBuffer));
    
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
    auto eye = DirectX::XMVectorSet(0.0f, 0.7f, 7.0f, 0.f);
    auto at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.f);
    auto up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);
    DirectX::XMStoreFloat4x4(&m_constantData.view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(eye, at, up)));
    auto viewportSize = m_deviceResources.GetViewportSize();
    float aspectRatioX = static_cast<float>(viewportSize.cx) / static_cast<float>(viewportSize.cy);
    float aspectRatioY = aspectRatioX < (16.0f / 9.0f) ? aspectRatioX / (16.0f / 9.0f) : 1.0f;  //?
    float nearPlane = 0.01f;
    float farPlane = 100.0f;
    DirectX::XMStoreFloat4x4(&m_constantData.projection, DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovRH(2.0f * atan(tan(DirectX::XMConvertToRadians(70) * 0.5f) / aspectRatioY), aspectRatioX, nearPlane, farPlane)));
}

void Renderer::Update() {
    DirectX::XMStoreFloat4x4(&m_constantData.world, DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians((float)m_frameCount++))));
    if (m_frameCount == MAXUINT) {
        m_frameCount = 0;
    }
}

void Renderer::Render() {
    auto deviceContext = m_deviceResources.GetDeviceContext();
    auto renderTargetView = m_deviceResources.GetRenderTargetView();
    auto depthStencilView = m_deviceResources.GetDepthStencilView();

    deviceContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &m_constantData, 0, 0);
    // clear
    float blue = static_cast<float>(abs(sin(static_cast<double>(m_frameCount) * 3.141592 / 100.0)));
    D3D11_VIDEO_COLOR_RGBA color{ 0.071f, 0.04f, blue, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, reinterpret_cast<float*>(&color));
    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    // IA stage
    UINT stride = sizeof(VertexPositionColor);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
    deviceContext->IASetInputLayout(m_pInputLayout.Get());
    deviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // Vertex Shader stage
    deviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    deviceContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
    // Pixel Shader stage
    deviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
    // OM stage
    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
    // Draw
    deviceContext->DrawIndexed(m_indexCount, 0, 0);
}


