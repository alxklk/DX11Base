#include "stdafx.h"
#include "Renderer.h"


CRenderer::CRenderer()
{
}


CRenderer::~CRenderer()
{
}

bool CRenderer::Init(SAppState& appState)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = appState.width;
	swapChainDesc.BufferDesc.Height = appState.height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = appState.win;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.Flags = 0;

	UINT createDeviceFlags = 0;

	//		createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &appState.d3dSwapChain, &appState.d3dDevice,
		&featureLevel, &appState.d3dDeviceContext);
	if (FAILED(hr))
	{
		printf_s("D3D Device failed to init\n", featureLevel);
		return false;
	}

	printf_s("D3D Device Feature Level %04x\n", featureLevel);

	ID3D11Texture2D* backBuffer;
	hr = appState.d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	hr = appState.d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &appState.d3dRenderTargetView);
	if (FAILED(hr))
	{
		return false;
	}


	SafeRelease(backBuffer);

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	memset(&depthStencilBufferDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = appState.width;
	depthStencilBufferDesc.Height = appState.height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = appState.d3dDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &appState.d3dDepthStencilBuffer);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	hr = appState.d3dDevice->CreateDepthStencilView(appState.d3dDepthStencilBuffer, nullptr, &appState.d3dDepthStencilView);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	memset(&depthStencilStateDesc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = FALSE;

	hr = appState.d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &appState.d3dDepthStencilState);

	appState.d3dDeviceContext->OMSetRenderTargets(1, &appState.d3dRenderTargetView, NULL);
	appState.d3dDeviceContext->OMSetDepthStencilState(appState.d3dDepthStencilState, 1);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = FALSE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	hr = appState.d3dDevice->CreateRasterizerState(&rasterizerDesc, &appState.d3dRasterizerState);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}


	D3D11_BUFFER_DESC constantBufferDesc;
	memset(&constantBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = appState.d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &appState.constantBuffers[0]);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}
	hr = appState.d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &appState.constantBuffers[1]);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}
	hr = appState.d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &appState.constantBuffers[2]);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	return true;
}

