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
	bool reloadShaders;
	int frameStamp;
	SMouseEvent lastMouseEvent;

	SAppState()
		: width(1800)
		, height(1000)
		, quitRequested(false)
		, reloadShaders(true)
		, frameStamp(0)
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
		gAppState.lastMouseEvent.frame=gAppState.frameStamp;
	}
	else if(imsg==WM_DESTROY)
	{
		stop(hwnd, imsg, wpar, lpar);
	}
	else if(imsg==WM_KEYDOWN)
	{
		if(wpar==VK_F5)
		{
			printf("Reload shaders\n");
			gAppState.reloadShaders=true;
		}
		else if(wpar==VK_ESCAPE)
		{
			printf("Reload shaders\n");
			gAppState.quitRequested=true;
		}
	}
	else if(imsg==WM_KEYUP)
	{
	}
	return DefWindowProc(hwnd, imsg, wpar, lpar);
}

namespace teapot
{
#include "teapot.h"
}

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

	CScene* scene=new CScene;

	CRTTexture* texP1=new CRTTexture;
	texP1->Create(renderer->GetDevice(), gAppState.width, gAppState.height, DXGI_FORMAT_R8G8B8A8_UNORM);
	CRTTexture* texP2=new CRTTexture;
	texP2->Create(renderer->GetDevice(), gAppState.width, gAppState.height, DXGI_FORMAT_R8G8B8A8_UNORM);
	CRTTexture* texP0=new CRTTexture;
	texP0->Create(renderer->GetDevice(), gAppState.width, gAppState.height, DXGI_FORMAT_R8G8B8A8_UNORM);


	CQuad* quad=new CQuad;
	quad->Create(renderer->GetDevice(),0.9f);
	quad->SetShaderSetup("quad");
	quad->SetTextureView(0,0);
	quad->SetTextureView(1,0);
	quad->SetRTView(texP0->GetRTView());
	scene->AddModel(quad);

	CModel* model=new CModel;
	model->Create(renderer->GetDevice(),
		(VertexPosNormTexUV*)teapot::vb, sizeof(teapot::vb)/sizeof(VertexPosNormTexUV),
		teapot::ib, sizeof(teapot::ib)/sizeof(unsigned short)
		);
	model->SetTextureView(0,0);
	model->SetRTView(0);
	scene->AddModel(model);

	
	CQuad* quadP1=new CQuad;
	quadP1->Create(renderer->GetDevice());
	quadP1->SetShaderSetup("quad_p1");
	quadP1->SetTextureView(0,texP0->GetShaderResourceView());
	quadP1->SetRTView(texP1->GetRTView());
	scene->AddModel(quadP1);

	CQuad* quadP2=new CQuad;
	quadP2->Create(renderer->GetDevice());
	quadP2->SetShaderSetup("quad_p2");
	quadP2->SetTextureView(0,texP1->GetShaderResourceView());
	quadP2->SetRTView(texP2->GetRTView());
	scene->AddModel(quadP2);
	
	CQuad* quadCombine=new CQuad;
	quadCombine->Create(renderer->GetDevice());
	quadCombine->SetShaderSetup("quad_combine");
	quadCombine->SetTextureView(0,texP0->GetShaderResourceView());
	quadCombine->SetTextureView(1,texP2->GetShaderResourceView());
	quadCombine->SetRTView(renderer->GetRTView());
	scene->AddModel(quadCombine);
	

	while(1)
	{
		/*		while(!isPending())
				{
				SleepEx(1, TRUE);
				}*/
		while(pollQueue())
		{
			if(gAppState.quitRequested)
				break;
		}
		if(gAppState.quitRequested)
			break;
		if(gAppState.reloadShaders)
		{
			gAppState.reloadShaders=false;
			{
				D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[]=
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosTexUV, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosTexUV, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
				};
				renderer->CreateShaderSetup("quad", L"..\\data\\shaders\\ScreenQuadVS.hlsl", L"..\\data\\shaders\\ScreenQuadPS.hlsl", vertexLayoutDesc, 2, sizeof(float)*8);
			}

			{
				D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[]=
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosTexUV, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosTexUV, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
				};
				renderer->CreateShaderSetup("quad_p1", L"..\\data\\shaders\\ScreenQuadVS.hlsl", L"..\\data\\shaders\\ScreenQuadPass1PS.hlsl", vertexLayoutDesc, 2, sizeof(float)*8);
			}

			{
				D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[]=
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosTexUV, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosTexUV, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
				};
				renderer->CreateShaderSetup("quad_p2", L"..\\data\\shaders\\ScreenQuadVS.hlsl", L"..\\data\\shaders\\ScreenQuadPass2PS.hlsl", vertexLayoutDesc, 2, sizeof(float)*8);
			}

			{
				D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[]=
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosTexUV, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosTexUV, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
				};
				renderer->CreateShaderSetup("quad_combine", L"..\\data\\shaders\\ScreenQuadVS.hlsl", L"..\\data\\shaders\\ScreenQuadCombinePS.hlsl", vertexLayoutDesc, 2, sizeof(float)*8);
			}

			{
				D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[]=
				{
					{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosNormTexUV, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosNormTexUV, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
					{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosNormTexUV, uv), D3D11_INPUT_PER_VERTEX_DATA, 0}
				};
				renderer->CreateShaderSetup("model", L"..\\data\\shaders\\RenderModelVS.hlsl", L"..\\data\\shaders\\RenderModelPS.hlsl", vertexLayoutDesc, 3, sizeof(float)*16);
			}
		}

		struct
		{
			float sz[4];
			float mp[4];
		}cb_quad;
		cb_quad.sz[0]=(float)gAppState.width;
		cb_quad.sz[1]=(float)gAppState.height;
		cb_quad.mp[0]=(float)gAppState.lastMouseEvent.x;
		cb_quad.mp[1]=(float)gAppState.lastMouseEvent.y;


		renderer->UseShaderSetup("quad");
		renderer->UpdateShaderConstants((void*)&cb_quad);
		renderer->UseShaderSetup("quad_p1");
		renderer->UpdateShaderConstants((void*)&cb_quad);
		renderer->UseShaderSetup("quad_p2");
		renderer->UpdateShaderConstants((void*)&cb_quad);
		renderer->UseShaderSetup("quad_combine");
		renderer->UpdateShaderConstants((void*)&cb_quad);


		static struct S_CBModel
		{
			XMMATRIX wm;
			S_CBModel() { wm=XMMatrixIdentity(); }
		}cb_model;


		if(gAppState.lastMouseEvent.frame==gAppState.frameStamp)
		{
			if(gAppState.lastMouseEvent.button_l)
			{
				float dy=gAppState.lastMouseEvent.delta_y/100.0f;
				float dx=gAppState.lastMouseEvent.delta_x/100.0f;

//				cb_model.wm=XMMatrixMultiply(XMMatrixRotationX(dy), cb_model.wm);
//				cb_model.wm=XMMatrixMultiply(XMMatrixRotationY(dx), cb_model.wm);

				float l=sqrt(dx*dx+dy*dy);

				XMFLOAT4 rotQuatV4(-dy*sin(l), dx*sin(l), 0, cos(l));
				XMVECTOR rotQuat=XMLoadFloat4(&rotQuatV4);
				rotQuat=XMQuaternionNormalize(rotQuat);
				cb_model.wm=XMMatrixMultiply(cb_model.wm, XMMatrixRotationQuaternion(rotQuat));
			}
		}
		//		for(int i=0; i<16; i++)
//			cb_model.wm[i]=wrldm.r[i/4].m128_f32[i%4];
		renderer->UseShaderSetup("model");
		renderer->UpdateShaderConstants((void*)&cb_model);


		renderer->RenderScene(scene);
		gAppState.frameStamp++;

	}

	renderer->Done();

	::OutputDebugString(L"Quiting...\n");

	return 0;
}
