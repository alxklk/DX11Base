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

class IModel
{
public:
	virtual ID3D11Buffer *const GetVertexBuffer()=0;
	virtual ID3D11Buffer* GetIndexBuffer()const=0;
	virtual const char* GetShaderSetup()const=0;
	virtual int GetVertexStride()const=0;
};

class CModel : public IModel
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
public:
	void Create(ID3D11Device* device, VertexPosColor* vertices, int nVertices, WORD* indices, int nIndices);
	CModel::CModel()
	: vertexBuffer(nullptr)
	, indexBuffer(nullptr)
	{}
	ID3D11Buffer *const GetVertexBuffer()const {return vertexBuffer;}
	ID3D11Buffer* GetIndexBuffer ()const {return indexBuffer;}
	virtual int GetVertexStride(){return sizeof(VertexPosColor);}
	virtual ~CModel()
	{
	};
};

class CQuad : public IModel
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
public:
	void Create(ID3D11Device* device, VertexPosColor* vertices, int nVertices, WORD* indices, int nIndices);
	ID3D11Buffer *const GetVertexBuffer()const {return vertexBuffer;}
	ID3D11Buffer* GetIndexBuffer ()const {return indexBuffer;}
	int GetVertexStride(){return sizeof(VertexPosTexUV);}
	bool Create();
	virtual ~CQuad()
	{
	}
};
