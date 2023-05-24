#pragma once

#include <memory>
#include <utility>
#include "stdafx.h"
#include "DeviceResources.h"
#include "Registry.h"

using Microsoft::WRL::ComPtr;


class Renderer{
public:
    explicit Renderer(DeviceResources *dr);
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

    DeviceResources *m_pDeviceResources;
    Registry m_registry{3, 2};
    ComPtr<ID3D11Buffer> m_pVertexBuffer;
    ComPtr<ID3D11VertexShader> m_pVertexShader;
    ComPtr<ID3D11InputLayout> m_pInputLayout;
    ComPtr<ID3D11PixelShader> m_pPixelShader;
    ComPtr<ID3D11Buffer> m_pConstantBuffer;
    UINT m_frameCount = 0;
    ConstantData m_constantData{};
    DirectX::XMFLOAT2 m_center{0.1, 0.2};
};

