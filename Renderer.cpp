#include "stdafx.h"
#include "Renderer.h"

using namespace DirectX;

bool CRenderer::Init(HWND win)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(swapChainDesc));

	RECT clientRect;
	GetClientRect(win, &clientRect);

	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;


	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = win;
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
		D3D11_SDK_VERSION, &swapChainDesc, &d3dSwapChain, &d3dDevice,
		&featureLevel, &d3dDeviceContext);
	if (FAILED(hr))
	{
		printf_s("D3D Device failed to init\n", featureLevel);
		return false;
	}

	printf_s("D3D Device Feature Level %04x\n", featureLevel);

	ID3D11Texture2D* backBuffer;
	hr = d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	hr = d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &d3dRenderTargetView);
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
	depthStencilBufferDesc.Width = width;
	depthStencilBufferDesc.Height = height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = d3dDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &d3dDepthStencilBuffer);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	hr = d3dDevice->CreateDepthStencilView(d3dDepthStencilBuffer, nullptr, &d3dDepthStencilView);
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

	hr = d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &d3dDepthStencilState);

	d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, NULL);
	d3dDeviceContext->OMSetDepthStencilState(d3dDepthStencilState, 1);

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

	hr = d3dDevice->CreateRasterizerState(&rasterizerDesc, &d3dRasterizerState);
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

	hr = d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffers[0]);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}
	hr = d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffers[1]);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}
	hr = d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffers[2]);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	return true;
}


void CRenderer::RenderScene(CScene* scene)
{
	int x=0;
	int y=0;
	float clearColor[4] = { 0.2f,
		0.2f + 0.1f*(y & 0xff) / 255.0f,
		0.2f + 0.1f*(x & 0xff) / 255.0f, 1 };
	float clearDepth = 0.0f;
	UINT8 clearStencil = 0;
	d3dDeviceContext->ClearRenderTargetView(d3dRenderTargetView, clearColor);
	d3dDeviceContext->ClearDepthStencilView(d3dDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, clearDepth, clearStencil);

	const UINT vertexStride = sizeof(VertexPosTexUV);
	const UINT offset = 0;


	d3dDeviceContext->IASetVertexBuffers(0, 1, fxquad->GetVertexBuffer(), &vertexStride, &offset);
	d3dDeviceContext->IASetInputLayout(fxquad->inputLayout);
	d3dDeviceContext->IASetIndexBuffer(fxquad->indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dDeviceContext->VSSetShader(fxquad.vertexShader, nullptr, 0);
	d3dDeviceContext->VSSetConstantBuffers(0, 3, constantBuffers);

	d3dDeviceContext->RSSetState(d3dRasterizerState);
	d3dDeviceContext->RSSetViewports(1,&appState.Viewport);

	d3dDeviceContext->PSSetShader(fxquad.pixelShader, nullptr, 0);

	d3dDeviceContext->DrawIndexed(6, 0, 0);

	d3dSwapChain->Present(0, 0);
}

bool CRenderer::Done()
{
	SafeRelease(d3dDevice);
	return true;
}

bool CRenderer::CreateRenderSetup()
{
	ID3DBlob* psBlob = 0;
	ID3DBlob* vsBlob = 0;
	model.vertexShader = LoadVertexShader(appState, L"..\\data\\shaders\\RenderModelVS.hlsl", "main", "vs_4_0", &vsBlob);
	model.pixelShader = LoadPixelShader(appState, L"..\\data\\shaders\\RenderModelPS.hlsl", "main", "ps_4_0");

	// Create the input layout for the vertex shader.
	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, col), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &model.inputLayout);
	if (FAILED(hr))
	{
		return false;
	}
}



ID3DBlob* LoadShader(const _TCHAR* fileName, const char* entryPoint, const char* profile)
{
	ID3DBlob* pShaderBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT hr = D3DCompileFromFile(fileName, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile,
		flags, 0, &pShaderBlob, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			char* errorMessage = (char*)pErrorBlob->GetBufferPointer();
			OutputDebugStringA(errorMessage);
			SafeRelease(pShaderBlob);
			SafeRelease(pErrorBlob);
		}
		return 0;
	}
	return pShaderBlob;
}


ID3D11VertexShader* LoadVertexShader(SAppState& appState, const _TCHAR* fileName, const char* entryPoint, const char* profile, ID3DBlob** shaderBlob)
{
	ID3D11VertexShader* newShader = nullptr;

	if (*shaderBlob = LoadShader(fileName, entryPoint, profile))
	{
		g_AppState.renderer->GetDevice()->CreateVertexShader((*shaderBlob)->GetBufferPointer(), (*shaderBlob)->GetBufferSize(), nullptr, &newShader);
	}
	return newShader;
}

ID3D11PixelShader* LoadPixelShader(SAppState& appState, const _TCHAR* fileName, const char* entryPoint, const char* profile)
{
	ID3D11PixelShader* newShader = nullptr;

	if (ID3DBlob* shaderBlob = LoadShader(fileName, entryPoint, profile))
	{
		g_AppState.renderer->GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &newShader);
	}
	return newShader;
}
