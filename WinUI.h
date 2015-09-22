#pragma once 
#include <Windows.h>

union XYLPARAM
{
	unsigned int l;
	struct
	{
		short int x;
		short int y;
	};
	XYLPARAM(LPARAM I) :l((unsigned int)I) {};
};

struct SMouseEvent
{
	static const int MET_LBUTTONDOWN=0;
	static const int MET_MBUTTONDOWN=1;
	static const int MET_RBUTTONDOWN=2;
	static const int MET_LBUTTONUP=3;
	static const int MET_MBUTTONUP=4;
	static const int MET_RBUTTONUP=5;
	static const int MET_MOVE=6;
	static const int MET_WHEEL=7;
	int type;
	int x;
	int y;
	int delta_x;
	int delta_y;
	int drag_dx;
	int drag_dy;

	int wheel;
	bool button_l;
	bool button_r;
	bool button_m;
	bool key_alt;
	bool key_shift;
	bool key_ctrl;
};

inline bool ProcessMouseEvent(HWND hwnd, unsigned int imsg, WPARAM wpar, LPARAM lpar, SMouseEvent& destEvent)
{
	bool btn_l=(0!=(wpar&MK_LBUTTON));
	bool btn_r=(0!=(wpar&MK_RBUTTON));
	bool btn_m=(0!=(wpar&MK_MBUTTON));
	bool key_ctrl=(0!=(wpar&MK_CONTROL));
	bool key_shift=(0!=(wpar&MK_SHIFT));
	bool key_alt=(GetAsyncKeyState(VK_MENU)&0x8000)!=0;

	int x=XYLPARAM(lpar).x;
	int y=XYLPARAM(lpar).y;

	static int prev_x=x;;
	static int prev_y=y;

	static int drag_startx=0;
	static int drag_starty=0;

	switch(imsg)
	{
	case WM_LBUTTONDOWN:destEvent.type=SMouseEvent::MET_LBUTTONDOWN; SetCapture(hwnd); break;
	case WM_LBUTTONUP:destEvent.type=SMouseEvent::MET_LBUTTONDOWN; ReleaseCapture(); break;
	case WM_RBUTTONDOWN:destEvent.type=SMouseEvent::MET_LBUTTONDOWN; SetCapture(hwnd); break;
	case WM_RBUTTONUP:destEvent.type=SMouseEvent::MET_LBUTTONDOWN; ReleaseCapture(); break;
	case WM_MBUTTONDOWN:destEvent.type=SMouseEvent::MET_LBUTTONDOWN; SetCapture(hwnd); break;
	case WM_MBUTTONUP:destEvent.type=SMouseEvent::MET_LBUTTONDOWN; ReleaseCapture(); break;

	case WM_MOUSEWHEEL:
		destEvent.type=SMouseEvent::MET_WHEEL;
		destEvent.wheel=GET_WHEEL_DELTA_WPARAM(wpar)/WHEEL_DELTA;
		break;
	case WM_MOUSEMOVE:
		destEvent.type=SMouseEvent::MET_MOVE;
		break;
	default: return false;
	}

	destEvent.button_l=btn_l;
	destEvent.button_r=btn_r;
	destEvent.button_m=btn_m;
	destEvent.key_ctrl=key_ctrl;
	destEvent.key_shift=key_shift;
	destEvent.key_alt=key_alt;


	destEvent.x=x;
	destEvent.y=y;
	destEvent.delta_x=x-prev_x;
	destEvent.delta_y=y-prev_y;

	prev_x=x;
	prev_y=y;

	return true;
}

inline bool isPending()
{
	MSG msg={0, 0, 0, 0, 0, {0, 0}};
	if(PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
		return true;
	else
		return false;
}

inline bool pollQueue()
{
	MSG msg={0, 0, 0, 0, 0, {0, 0}};
	if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return true;
	}
	return false;
}

