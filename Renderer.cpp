#include "stdafx.h"
#include "Renderer.h"

using namespace DirectX;

bool CRenderer::Init(HWND win)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0, sizeof(swapChainDesc));

	RECT clientRect;
	GetClientRect(win, &clientRect);

	int width=clientRect.right-clientRect.left;
	int height=clientRect.bottom-clientRect.top;

	Viewport.Width=(float)width;
	Viewport.Height=(float)height;

	swapChainDesc.BufferCount=1;
	swapChainDesc.BufferDesc.Width=width;
	swapChainDesc.BufferDesc.Height=height;
	swapChainDesc.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator=60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator=1;
	swapChainDesc.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow=win;
	swapChainDesc.SampleDesc.Count=1;
	swapChainDesc.SampleDesc.Quality=0;
	swapChainDesc.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed=TRUE;
	swapChainDesc.Flags=0;

	UINT createDeviceFlags=0;

	for(int i=0;i<30;i++)
	{
		if(::GetAsyncKeyState(VK_SHIFT))
		{
			MessageBeep(0xFFFFFFFF);
			printf("Debug runtime selected\n");
			createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
			break;
		}
		Sleep(10);
	}
	D3D_FEATURE_LEVEL featureLevels[]=
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

	hr=D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &d3dSwapChain, &d3dDevice,
		&featureLevel, &d3dDeviceContext);
	if(FAILED(hr))
	{
		printf_s("D3D Device failed to init\n");
		return false;
	}

	printf_s("D3D Device Feature Level %04x\n", featureLevel);

	ID3D11Texture2D* backBuffer;
	hr=d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if(FAILED(hr))
	{
		return false;
	}

	hr=d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &d3dRenderTargetView);
	if(FAILED(hr))
	{
		return false;
	}

	SafeRelease(backBuffer);

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	memset(&depthStencilBufferDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize=1;
	depthStencilBufferDesc.BindFlags=D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags=0;
	depthStencilBufferDesc.Format=DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width=width;
	depthStencilBufferDesc.Height=height;
	depthStencilBufferDesc.MipLevels=1;
	depthStencilBufferDesc.SampleDesc.Count=1;
	depthStencilBufferDesc.SampleDesc.Quality=0;
	depthStencilBufferDesc.Usage=D3D11_USAGE_DEFAULT;

	hr=d3dDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &d3dDepthStencilBuffer);
	if(FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	hr=d3dDevice->CreateDepthStencilView(d3dDepthStencilBuffer, nullptr, &d3dDepthStencilView);
	if(FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	memset(&depthStencilStateDesc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable=FALSE;
	depthStencilStateDesc.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc=D3D11_COMPARISON_GREATER;
	depthStencilStateDesc.StencilEnable=FALSE;

	hr=d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &d3dDepthStencilState);
	d3dDeviceContext->OMSetDepthStencilState(d3dDepthStencilState, 1);


	D3D11_SAMPLER_DESC sampDesc;
	memset(&sampDesc, 0, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;    
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = d3dDevice->CreateSamplerState(&sampDesc, &d3dSamplerState);



	D3D11_RASTERIZER_DESC rasterizerDesc;
	memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable=FALSE;
	rasterizerDesc.CullMode=D3D11_CULL_NONE;
	rasterizerDesc.DepthBias=0;
	rasterizerDesc.DepthBiasClamp=0.0f;
	rasterizerDesc.DepthClipEnable=FALSE;
	rasterizerDesc.FillMode=D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise=FALSE;
	rasterizerDesc.MultisampleEnable=FALSE;
	rasterizerDesc.ScissorEnable=FALSE;
	rasterizerDesc.SlopeScaledDepthBias=0.0f;

	hr=d3dDevice->CreateRasterizerState(&rasterizerDesc, &d3dRasterizerState);
	if(FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}
	return true;
}



bool CRenderer::UseShaderSetup(const char* shaderSetup)
{
	TShaderSterupMap::iterator foundShaderSetup=shaderSetupMap.find(shaderSetup);
	if(foundShaderSetup==shaderSetupMap.end())
	{
		currentShaderSetup=0;
		return false;
	}
	currentShaderSetup=foundShaderSetup->second;

	d3dDeviceContext->IASetInputLayout(currentShaderSetup->inputLayout);
	d3dDeviceContext->VSSetShader(currentShaderSetup->vertexShader, nullptr, 0);
	d3dDeviceContext->PSSetShader(currentShaderSetup->pixelShader, nullptr, 0);
	if(currentShaderSetup->constantBuffer)
		d3dDeviceContext->PSSetConstantBuffers(0,1,&currentShaderSetup->constantBuffer);
//	d3dDeviceContext->VSSetConstantBuffers(2,1,&currentShaderSetup->constantBuffer);
	return true;
}

bool CRenderer::UpdateShaderConstants(void* constantBuffer)
{
	ID3D11Buffer* cb=0;
	if(currentShaderSetup&&currentShaderSetup->constantBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE mr;
		HRESULT hr=d3dDeviceContext->Map(currentShaderSetup->constantBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mr);
		if(FAILED(hr))
			return false;
		for(int i=0;i<currentShaderSetup->constantsSize;i++)
			((char*)mr.pData)[i]=((char*)constantBuffer)[i];
		d3dDeviceContext->Unmap(currentShaderSetup->constantBuffer,0);


//		d3dDeviceContext->UpdateSubresource(currentShaderSetup->constantBuffer, 0, 0, &constantBuffer, 0, 0);
//		d3dDeviceContext->PSSetConstantBuffers(0,1,&currentShaderSetup->constantBuffer);

		return true;
	}
	return false;
}


void CRenderer::RenderScene(CScene* scene)
{
	int x=0;
	int y=0;
	float clearColor[4]={0.2f,
		0.2f+0.1f*(y&0xff)/255.0f,
		0.2f+0.1f*(x&0xff)/255.0f, 1};
	float clearDepth=0.0f;
	UINT8 clearStencil=0;
	d3dDeviceContext->ClearRenderTargetView(d3dRenderTargetView, clearColor);
	d3dDeviceContext->ClearDepthStencilView(d3dDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, clearDepth, clearStencil);


	d3dDeviceContext->RSSetState(d3dRasterizerState);
	d3dDeviceContext->RSSetViewports(1, &Viewport);
	d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, NULL);

	size_t modelIndex=0;
	while(true)
	{
		IModel* model=scene->GetModel(modelIndex);
		if(model==nullptr)
			break;

		const char* shaderSetup=model->GetShaderSetup();
		if(shaderSetup)
			UseShaderSetup(shaderSetup);

		if(model->GetIndexBuffer()&&model->GetVertexBuffer())
		{
			d3dDeviceContext->IASetIndexBuffer(model->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
			d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			ID3D11Buffer*const  vertexBuffer=model->GetVertexBuffer();
			const UINT offset=0;
			const UINT vertexStride=model->GetVertexStride();
			d3dDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &offset);

			ID3D11RenderTargetView* RTView=model->GetRTView();
			if(RTView)
			{
				ID3D11ShaderResourceView* zero=0;
				d3dDeviceContext->PSSetShaderResources(0,1,&zero);
				d3dDeviceContext->OMSetRenderTargets(1,&RTView,d3dDepthStencilView);
			}

			ID3D11ShaderResourceView* textureView=model->GetTextureView();
			if(textureView)
			{
				d3dDeviceContext->PSSetSamplers(0,1,&d3dSamplerState);
				d3dDeviceContext->PSSetShaderResources(0,1,&textureView);
			}


			d3dDeviceContext->DrawIndexed(model->GetIndexCount(), 0, 0);
		}



		modelIndex++;
	}

	d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, d3dDepthStencilView);

	d3dSwapChain->Present(0, 0);
}

bool CRenderer::Done()
{
	SafeRelease(d3dDevice);
	return true;
}


bool CRenderer::CreateShaderSetup(const char* setupName, const _TCHAR* vertexShaderFile, const _TCHAR* pixelShaderFile,
	const D3D11_INPUT_ELEMENT_DESC* layoutDesc, int nVL, int constantsSize)
{
	ID3DBlob* psBlob=0;
	ID3DBlob* vsBlob=0;
	ID3D11InputLayout* inputLayout=0;

	ID3D11VertexShader* vertexShader=LoadVertexShader(vertexShaderFile, "main", "vs_4_0", &vsBlob);
	ID3D11PixelShader* pixelShader=LoadPixelShader(pixelShaderFile, "main", "ps_4_0");

	HRESULT hr;
	hr=d3dDevice->CreateInputLayout(layoutDesc, nVL, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
	if(FAILED(hr))
	{
		return false;
	}

	ID3D11Buffer* constantBuffer = 0;

	if(constantsSize)
	{
		D3D11_BUFFER_DESC constantBufferDesc;
		memset(&constantBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

		constantBufferDesc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.ByteWidth=constantsSize;
		constantBufferDesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.Usage=D3D11_USAGE_DYNAMIC;

		hr=d3dDevice->CreateBuffer(&constantBufferDesc, 0, &constantBuffer);
		if(FAILED(hr))
		{
			printf("Constant Buffer create failed\n");
			return false;
		}
	}

	SShaderSetup* setup=new SShaderSetup;
	setup->inputLayout=inputLayout;
	setup->pixelShader=pixelShader;
	setup->vertexShader=vertexShader;
	setup->constantBuffer=constantBuffer;
	setup->constantsSize=constantsSize;
	shaderSetupMap[setupName]=setup;
	return true;
}

ID3DBlob* LoadShader(const _TCHAR* fileName, const char* entryPoint, const char* profile)
{
	ID3DBlob* pShaderBlob=nullptr;
	ID3DBlob* pErrorBlob=nullptr;
	UINT flags=D3DCOMPILE_ENABLE_STRICTNESS;

	HRESULT hr=D3DCompileFromFile(fileName, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile,
		flags, 0, &pShaderBlob, &pErrorBlob);

	if(FAILED(hr))
	{
		if(pErrorBlob)
		{
			char* errorMessage=(char*)pErrorBlob->GetBufferPointer();
			OutputDebugStringA(errorMessage);
			SafeRelease(pShaderBlob);
			SafeRelease(pErrorBlob);
		}
		return 0;
	}
	return pShaderBlob;
}

ID3D11VertexShader* CRenderer::LoadVertexShader(const _TCHAR* fileName, const char* entryPoint, const char* profile, ID3DBlob** shaderBlob)
{
	ID3D11VertexShader* newShader=nullptr;

	if(*shaderBlob=LoadShader(fileName, entryPoint, profile))
	{
		d3dDevice->CreateVertexShader((*shaderBlob)->GetBufferPointer(), (*shaderBlob)->GetBufferSize(), nullptr, &newShader);
	}
	return newShader;
}

ID3D11PixelShader* CRenderer::LoadPixelShader(const _TCHAR* fileName, const char* entryPoint, const char* profile)
{
	ID3D11PixelShader* newShader=nullptr;

	if(ID3DBlob* shaderBlob=LoadShader(fileName, entryPoint, profile))
	{
		d3dDevice->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &newShader);
	}
	return newShader;
}
