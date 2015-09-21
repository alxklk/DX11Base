#pragma once

#include "Model.h"
#include "Scene.h"

struct SShaderSetup
{
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;
};


class CRenderer
{
	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;
	IDXGISwapChain* d3dSwapChain;

	ID3D11RenderTargetView* d3dRenderTargetView;
	ID3D11DepthStencilView* d3dDepthStencilView;
	ID3D11Texture2D* d3dDepthStencilBuffer;

	ID3D11DepthStencilState* d3dDepthStencilState;
	ID3D11RasterizerState* d3dRasterizerState;
	D3D11_VIEWPORT Viewport;

	ID3D11Buffer* constantBuffers[3];

	void RenderModel(CModel* model)
	{

	}

	CQuad* fxquad;

public:
	ID3D11Device* GetDevice()
	{
		if (!d3dDevice)
			::DebugBreak();
		return d3dDevice;
	}

	std::map

	void RenderScene(SScene* scene);

	bool CreateDevice();

	CRenderer()
		:d3dDevice(nullptr)
		, d3dDeviceContext(nullptr)
		, d3dSwapChain(nullptr)
		, d3dRenderTargetView(nullptr)
		, d3dDepthStencilView(nullptr)
		, d3dDepthStencilBuffer(nullptr)
		, d3dDepthStencilState(nullptr)
		, d3dRasterizerState(nullptr)
	{
		constantBuffers[0] = nullptr;
		constantBuffers[1] = nullptr;
		constantBuffers[2] = nullptr;
		Viewport.Width = (float)g_AppState.width;
		Viewport.Height = (float)g_AppState.height;
		Viewport.TopLeftX = 0.0f;
		Viewport.TopLeftY = 0.0f;
		Viewport.MinDepth = 0.0f;
		Viewport.MaxDepth = 1.0f;
	}
};
