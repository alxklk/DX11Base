// MyTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Renderer.h"

using namespace DirectX;

struct SAppState
{
	CRenderer* renderer;
public:
	HWND win;
	int width;
	int height;
	bool quitRequested;
	SMouseEvent lastMouseEvent;
	CRenderer* GetRenderer(){ return renderer; }

	SAppState()
		: width(1800)
		, height(1000)
		, quitRequested(false)
	{

	}

}g_AppState;


template<typename T>
inline void SafeRelease(T& ptr)
{
	if (ptr != NULL)
	{
		ptr->Release();
		ptr = NULL;
	}
}


void stop(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar)
{
	PostQuitMessage(0);
	g_AppState.quitRequested = true;
	::OutputDebugString(L"stop issued\n");
}

LRESULT CALLBACK wp(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar)
{
	if (ProcessMouseEvent(hwnd, imsg, wpar, lpar, gAppState.lastMouseEvent))
	{
	}
	else if (imsg == WM_DESTROY)
	{
		stop(hwnd, imsg, wpar, lpar);
	}
	else if (imsg == WM_KEYDOWN)
	{
	}
	else if (imsg == WM_KEYUP)
	{
	}
	return DefWindowProc(hwnd, imsg, wpar, lpar);
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

struct SScene
{
//	SRenderModel object;
	CQuad fxquad;
}gScene;

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


VertexPosColor BoxVertices[8] =
{
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f) }
};

WORD BoxIndicies[36] =
{
	0, 1, 2, 0, 2, 3,
	4, 6, 5, 4, 7, 6,
	4, 5, 1, 4, 1, 0,
	3, 2, 6, 3, 6, 7,
	1, 5, 6, 1, 6, 2,
	4, 0, 3, 4, 3, 7
};


bool InitRender(SAppState& appState)
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
		appState.d3dDeviceContext->OMSetDepthStencilState( appState.d3dDepthStencilState, 1 );

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

CRenderer::Render(SScene* scene)
{
	float clearColor[4] = { 0.2f,
		0.2f + 0.1f*(g_AppState.lastMouseEvent.y & 0xff) / 255.0f,
		0.2f + 0.1f*(g_AppState.lastMouseEvent.x & 0xff) / 255.0f, 1 };
	float clearDepth = 0.0f;
	UINT8 clearStencil = 0;
	d3dDeviceContext->ClearRenderTargetView(appState.d3dRenderTargetView, clearColor);
	d3dDeviceContext->ClearDepthStencilView(appState.d3dDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, clearDepth, clearStencil);

	const UINT vertexStride = sizeof(VertexPosTexUV);
	const UINT offset = 0;


	appState.d3dDeviceContext->IASetVertexBuffers(0, 1, &gScene.fxquad.vertexBuffer, &vertexStride, &offset);
	appState.d3dDeviceContext->IASetInputLayout(gScene.fxquad.inputLayout);
	appState.d3dDeviceContext->IASetIndexBuffer(gScene.fxquad.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	appState.d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	appState.d3dDeviceContext->VSSetShader(gScene.fxquad.vertexShader, nullptr, 0);
	appState.d3dDeviceContext->VSSetConstantBuffers(0, 3, appState.constantBuffers);

	appState.d3dDeviceContext->RSSetState(appState.d3dRasterizerState);
	appState.d3dDeviceContext->RSSetViewports(1,&appState.Viewport);

	appState.d3dDeviceContext->PSSetShader(gScene.fxquad.pixelShader, nullptr, 0);

	appState.d3dDeviceContext->DrawIndexed(6, 0, 0);

	appState.d3dSwapChain->Present(0, 0);
}

bool DoneRender(SAppState& appState)
{
	SafeRelease(appState.d3dDevice);
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{

	HCURSOR arrowCursor = LoadCursor(0, IDC_ARROW);

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), 0, wp, 0, 0, 0, 0, 0, 0, 0, L"APP01" };
	wc.hCursor = arrowCursor;
	RegisterClassEx(&wc);

	RECT wrect = { 0, 0, gAppState.width, gAppState.height };
	AdjustWindowRectEx(&wrect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW);
	gAppState.win = CreateWindowEx(WS_EX_APPWINDOW, L"APP01", L"Masked Blur", WS_OVERLAPPEDWINDOW, -1920, 0,
		wrect.right - wrect.left, wrect.bottom - wrect.top, 0, 0, 0, 0);

	ShowWindow(gAppState.win, SW_SHOWNORMAL);
	UpdateWindow(gAppState.win);

	SetWindowLongPtr(gAppState.win, GWL_USERDATA, (LONG_PTR)&(gAppState));

	InitRender(gAppState);
//	CreateModel(gAppState, gScene.object);
	CreateQuad(gAppState, gScene.fxquad);

	while (1)
	{
		while (!isPending())
		{
			SleepEx(10, TRUE);
		}
		while (pollQueue())
		{
			if (gAppState.quitRequested)
				break;
		}
		if (gAppState.quitRequested)
			break;
		Render(gAppState);
	}

	::OutputDebugString(L"Quiting...\n");

	DoneRender(gAppState);

	return 0;
}

