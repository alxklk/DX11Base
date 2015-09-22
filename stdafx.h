// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <stddef.h>

// TODO: reference additional headers your program requires here

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <string>
#include <map>
#include <vector>

template<typename T>
inline void SafeRelease(T& ptr)
{
	if(ptr!=nullptr)
	{
		ptr->Release();
		ptr=nullptr;
	}
}

inline void SafeDelete(void* ptr)
{
	if(ptr!=nullptr)
	{
		delete ptr;
		ptr=nullptr;
	}
}

class AutoRelease
{
	std::vector<IUnknown*>items;
public:
	~AutoRelease()
	{
		for(size_t i=0; i < items.size(); i++)SafeRelease(items[i]);
		items.resize(0);
	}
	void AddItem(IUnknown* item) { items.push_back(item); }
};

class AutoDelete
{
	std::vector<void*>items;
public:
	~AutoDelete()
	{
		for(size_t i=0; i < items.size(); i++)SafeDelete(items[i]);
		items.resize(0);
	}
	void AddItem(void* item) { items.push_back(item); }
};



