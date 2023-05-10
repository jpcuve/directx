#pragma once
#include "stdafx.h"

using Microsoft::WRL::ComPtr;

class DeviceResources {
private:
	D3D_FEATURE_LEVEL m_featureLevel {D3D_FEATURE_LEVEL_1_0_CORE};
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<ID3D11Texture2D> m_pBackBuffer;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11Texture2D> m_pDepthStencil;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	SIZE m_viewportSize{0, 0};

	void InitDeviceResources();
	void InitWindowResources(HWND hWnd);
	void SetUp();
	void TearDown();
public:
    explicit DeviceResources(HWND hWnd);
	void Present();
	ID3D11Device* GetDevice() { return m_pDevice.Get(); };
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext.Get(); };
	ID3D11RenderTargetView* GetRenderTargetView() { return m_pRenderTargetView.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() { return m_pDepthStencilView.Get(); }
	SIZE GetViewportSize() { return m_viewportSize; }
};

