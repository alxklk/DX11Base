#pragma once

#include "stdafx.h"

#include "Model.h"
#include "Scene.h"


struct SShaderSetup
{
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* constantBuffer;
	int constantsSize;
};

class CRenderer
{
	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dDeviceContext;
	IDXGISwapChain* d3dSwapChain;

	ID3D11RenderTargetView* d3dRenderTargetView;
	ID3D11DepthStencilView* d3dDepthStencilView;
	ID3D11Texture2D* d3dDepthStencilBuffer;

	ID3D11DepthStencilState* d3dDepthStencilStateOff;
	ID3D11DepthStencilState* d3dDepthStencilStateOn;
	ID3D11RasterizerState* d3dRasterizerState;
	ID3D11SamplerState* d3dSamplerState;
	D3D11_VIEWPORT Viewport;

	void RenderModel(CModel* model);

	typedef std::map<std::string, SShaderSetup*> TShaderSterupMap;
	TShaderSterupMap shaderSetupMap;
	ID3D11VertexShader* LoadVertexShader(const _TCHAR* fileName, const char* entryPoint, const char* profile, ID3DBlob** shaderBlob);
	ID3D11PixelShader* LoadPixelShader(const _TCHAR* fileName, const char* entryPoint, const char* profile);

	SShaderSetup* currentShaderSetup;

	bool debug;

public:

	ID3D11Device* GetDevice()
	{
		if(!d3dDevice)
			::DebugBreak();
		return d3dDevice;
	}

	bool Init(HWND win);
	void RenderScene(CScene* scene);
	bool Done();

	bool CreateDevice();
	bool CreateShaderSetup(const char* setupName, const _TCHAR* vertexShaderFile, const _TCHAR* pixelShaderFile,
		const D3D11_INPUT_ELEMENT_DESC* layoutDesc, int nVL, int constantsSize=0);
	bool UseShaderSetup(const char* setupName);
	bool UpdateShaderConstants(void* constantBuffer=0);

	ID3D11RenderTargetView* GetRTView(){return d3dRenderTargetView;}

	CRenderer()
		: d3dDevice(nullptr)
		, d3dDeviceContext(nullptr)
		, d3dSwapChain(nullptr)
		, d3dRenderTargetView(nullptr)
		, d3dDepthStencilView(nullptr)
		, d3dDepthStencilBuffer(nullptr)
		, d3dDepthStencilStateOff(nullptr)
		, d3dDepthStencilStateOn(nullptr)
		, d3dRasterizerState(nullptr)
		, debug(false)
	{
		Viewport.Width=0;
		Viewport.Height=0;
		Viewport.TopLeftX=0.0f;
		Viewport.TopLeftY=0.0f;
		Viewport.MinDepth=0.0f;
		Viewport.MaxDepth=1.0f;
	}
	virtual ~CRenderer() {}
};
