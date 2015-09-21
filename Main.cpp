#include "stdafx.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Renderer.h"
#include "WinUI.h"

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

void stop(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar)
{
	PostQuitMessage(0);
	g_AppState.quitRequested = true;
	::OutputDebugString(L"stop issued\n");
}

LRESULT CALLBACK wp(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar)
{
	if (ProcessMouseEvent(hwnd, imsg, wpar, lpar, g_AppState.lastMouseEvent))
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



struct SScene
{
//	SRenderModel object;
	CQuad fxquad;
}gScene;



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


int _tmain(int argc, _TCHAR* argv[])
{

	HCURSOR arrowCursor = LoadCursor(0, IDC_ARROW);

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), 0, wp, 0, 0, 0, 0, 0, 0, 0, L"APP01" };
	wc.hCursor = arrowCursor;
	RegisterClassEx(&wc);

	RECT wrect = { 0, 0, g_AppState.width, g_AppState.height };
	AdjustWindowRectEx(&wrect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW);
	g_AppState.win = CreateWindowEx(WS_EX_APPWINDOW, L"APP01", L"Masked Blur", WS_OVERLAPPEDWINDOW, -1920, 0,
		wrect.right - wrect.left, wrect.bottom - wrect.top, 0, 0, 0, 0);

	ShowWindow(g_AppState.win, SW_SHOWNORMAL);
	UpdateWindow(g_AppState.win);

	SetWindowLongPtr(g_AppState.win, GWL_USERDATA, (LONG_PTR)&(g_AppState));

	while (1)
	{
		while (!isPending())
		{
			SleepEx(10, TRUE);
		}
		while (pollQueue())
		{
			if (g_AppState.quitRequested)
				break;
		}
		if (g_AppState.quitRequested)
			break;
	}

	::OutputDebugString(L"Quiting...\n");

	return 0;
}

