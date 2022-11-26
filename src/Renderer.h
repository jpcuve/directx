#pragma once
#include "stdafx.h"
#include "DeviceResources.h"

using Microsoft::WRL::ComPtr;


class Renderer{
public:
    Renderer(DeviceResources& dr) : m_deviceResources{ dr } {}
	void Update();
	void Render();
    void InitDeviceDependent();
    void InitWindowSizeDependent();
private:
    void InitShaders();
    void InitBuffers();
    typedef struct {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4X4 view;
        DirectX::XMFLOAT4X4 projection;
    } ConstantData;

    DeviceResources& m_deviceResources;
    ComPtr<ID3D11Buffer> m_pVertexBuffer;
    ComPtr<ID3D11Buffer> m_pIndexBuffer;
    ComPtr<ID3D11VertexShader> m_pVertexShader;
    ComPtr<ID3D11InputLayout> m_pInputLayout;
    ComPtr<ID3D11PixelShader> m_pPixelShader;
    ComPtr<ID3D11Buffer> m_pConstantBuffer;
    UINT m_frameCount = 0;
    size_t m_indexCount = 0;
    ConstantData m_constantData{};
};

