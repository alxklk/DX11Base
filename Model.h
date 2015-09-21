#pragma once

#include "stdafx.h"

using namespace DirectX;

struct VertexPosColor
{
	XMFLOAT3 pos;
	XMFLOAT3 col;
};

struct VertexPosTexUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class CModel
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
public:
	void Create(ID3D11Device* device, VertexPosColor* vertices, int nVertices, WORD* indices, int nIndices);
	CModel::CModel()
	: vertexBuffer(nullptr)
	, indexBuffer(nullptr)
	{}
	ID3D11Buffer* GetVertexBuffer(){return vertexBuffer;}
	ID3D11Buffer* GetIndexBuffer(){return indexBuffer;}
	int GetVertexStride(){return sizeof(VertexPosColor);}
	CModel::~CModel()
	{
	};
};

class CQuad : public CModel
{
public:
	int GetVertexStride(){return sizeof(VertexPosTexUV);}
	bool Create();
};
