#pragma once

#include "stdafx.h"

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
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
public:
	void Create(ID3D11Device* device, VertexPosColor* vertices, int nVertices, WORD* indices, int nIndices);
	CModel::CModel()
	:inputLayout(nullptr)
	, vertexBuffer(nullptr)
	, indexBuffer(nullptr)
	, vertexShader(nullptr)
	, pixelShader(nullptr)
	{}
	void AttachShaders(ID3D11VertexShader* vertex, ID3D11PixelShader* pixel)
	{
		vertexShader = vertex;
		pixelShader = pixel;
	}
	CModel::~CModel()
	{
	};
};

class CQuad : public CModel
{
public:
	bool Create();
};
