#include "stdafx.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Renderer.h"
#include "RTTexture.h"
#include "WinUI.h"

struct SAppState
{
public:
	HWND win;
	int width;
	int height;
	bool quitRequested;
	SMouseEvent lastMouseEvent;

	SAppState()
		: width(1800)
		, height(1000)
		, quitRequested(false)
	{

	}

}gAppState;

void stop(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar)
{
	PostQuitMessage(0);
	gAppState.quitRequested=true;
	::OutputDebugString(L"stop issued\n");
}

LRESULT CALLBACK wp(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar)
{
	if(ProcessMouseEvent(hwnd, imsg, wpar, lpar, gAppState.lastMouseEvent))
	{
	}
	else if(imsg==WM_DESTROY)
	{
		stop(hwnd, imsg, wpar, lpar);
	}
	else if(imsg==WM_KEYDOWN)
	{
	}
	else if(imsg==WM_KEYUP)
	{
	}
	return DefWindowProc(hwnd, imsg, wpar, lpar);
}

VertexPosColor BoxVertices[8]=
{
	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
	{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
	{XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
	{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
	{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
	{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
	{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f)}
};

WORD BoxIndicies[36]=
{
	0, 1, 2, 0, 2, 3,
	4, 6, 5, 4, 7, 6,
	4, 5, 1, 4, 1, 0,
	3, 2, 6, 3, 6, 7,
	1, 5, 6, 1, 6, 2,
	4, 0, 3, 4, 3, 7
};


int _tmain(int argc, _TCHAR* argv[])
{

	HCURSOR arrowCursor=LoadCursor(0, IDC_ARROW);

	WNDCLASSEX wc={sizeof(WNDCLASSEX), 0, wp, 0, 0, 0, 0, 0, 0, 0, L"APP01"};
	wc.hCursor=arrowCursor;
	RegisterClassEx(&wc);

	RECT wrect={0, 0, gAppState.width, gAppState.height};
	AdjustWindowRectEx(&wrect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW);
	gAppState.win=CreateWindowEx(WS_EX_APPWINDOW, L"APP01", L"Masked Blur", WS_OVERLAPPEDWINDOW, -1920, 0,
		wrect.right-wrect.left, wrect.bottom-wrect.top, 0, 0, 0, 0);

	ShowWindow(gAppState.win, SW_SHOWNORMAL);
	UpdateWindow(gAppState.win);

	SetWindowLongPtr(gAppState.win, GWL_USERDATA, (LONG_PTR)&(gAppState));

	CRenderer* renderer=new CRenderer;
	if(!renderer->Init(gAppState.win))
	{
		printf("Failed to initialize renderer\n");
		delete renderer;
		return 1;
	}


	{
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[]=
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosTexUV, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosTexUV, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		renderer->CreateShaderSetup("quad",
			L"..\\data\\shaders\\ScreenQuadVS.hlsl",
			L"..\\data\\shaders\\ScreenQuadPS.hlsl",
			vertexLayoutDesc, 2);
	}

	{
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[]=
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosTexUV, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosTexUV, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		renderer->CreateShaderSetup("quad_p1",
			L"..\\data\\shaders\\ScreenQuadPass1VS.hlsl",
			L"..\\data\\shaders\\ScreenQuadPass1PS.hlsl",
			vertexLayoutDesc, 2);
	}

	{
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[]=
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosTexUV, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosTexUV, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		renderer->CreateShaderSetup("quad_p2",
			L"..\\data\\shaders\\ScreenQuadPass2VS.hlsl",
			L"..\\data\\shaders\\ScreenQuadPass2PS.hlsl",
			vertexLayoutDesc, 2);
	}

	{
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[]=
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, col), D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		renderer->CreateShaderSetup("Model",
			L"..\\data\\shaders\\RenderModelVS.hlsl",
			L"..\\data\\shaders\\RenderModelPS.hlsl",
			vertexLayoutDesc, 2);
	}

	CScene* scene=new CScene;

	CRTTexture* texP1=new CRTTexture;
	texP1->Create(renderer->GetDevice(),gAppState.width, gAppState.height,DXGI_FORMAT_R8G8B8A8_UNORM);
	CRTTexture* texP2=new CRTTexture;
	texP2->Create(renderer->GetDevice(),gAppState.width, gAppState.height,DXGI_FORMAT_R8G8B8A8_UNORM);


	CQuad* quad_models=new CQuad;
	quad_models->Create(renderer->GetDevice());
	quad_models->SetShaderSetup(0);
	quad_models->SetTextureView(0);
	quad_models->SetRTView(texP1->GetRTView());
	CQuad* quad=new CQuad;
	quad->Create(renderer->GetDevice());
	quad->SetShaderSetup("quad");
	quad->SetTextureView(0);
	quad->SetRTView(texP1->GetRTView());
	CQuad* quadP1=new CQuad;
	quad->Create(renderer->GetDevice());
	quad->SetShaderSetup("quad");
	quad->SetTextureView(texP1->GetShaderResourceView());
	quad->SetRTView();
	CQuad* quadP2=new CQuad;
	quad->Create(renderer->GetDevice());
	quad->SetShaderSetup("quad");
	quad->SetTextureView();
	quad->SetRTView(renderer->GetRTView());
	scene->AddModel(quad);


	while(1)
	{
		while(!isPending())
		{
			SleepEx(10, TRUE);
		}
		while(pollQueue())
		{
			if(gAppState.quitRequested)
				break;
		}
		if(gAppState.quitRequested)
			break;
		renderer->RenderScene(scene);

	}

	renderer->Done();


	::OutputDebugString(L"Quiting...\n");

	return 0;
}




/*

ID3DBlob* vsBlob = 0;
model.vertexShader = LoadVertexShader(appState, L"..\\data\\shaders\\ScreenQuadVS.hlsl", "main", "vs_5_0", &vsBlob);
model.pixelShader = LoadPixelShader(appState, L"..\\data\\shaders\\ScreenQuadPS.hlsl", "main", "ps_5_0");

int posoffset = offsetof(VertexPosTexUV, pos);
int uvoffset = offsetof(VertexPosTexUV, uv);

D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
{
{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, posoffset, D3D11_INPUT_PER_VERTEX_DATA, 0 },
{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, uvoffset, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

hr = device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &model.inputLayout);
if (FAILED(hr))
{
printf("Fail\n");
return false;
}

ID3D11VertexShader* vertexShader = LoadVertexShader(L"..\\data\\shaders\\RenderModelVS.hlsl", "main", "vs_4_0", &vsBlob);
ID3D11PixelShader* pixelShader =   LoadPixelShader (L"..\\data\\shaders\\RenderModelPS.hlsl", "main", "ps_4_0");

// Create the input layout for the vertex shader.
D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
{
{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, col), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

ID3D11InputLayout* inputLayout;
HRESULT hr;
hr = d3dDevice->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
if (FAILED(hr))
{
return false;
}


*/


