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
	virtual ID3D11Buffer *const GetVertexBuffer()const=0;
	virtual ID3D11Buffer* GetIndexBuffer()const=0;
	virtual const char* GetShaderSetup()const=0;
	virtual int GetVertexStride()const=0;
};

class CModel : public IModel
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
public:
	bool Create(ID3D11Device* device, VertexPosColor* vertices, int nVertices, WORD* indices, int nIndices);
	CModel::CModel()
		: vertexBuffer(nullptr)
		, indexBuffer(nullptr)
	{
	}
	virtual ID3D11Buffer *const GetVertexBuffer()const { return vertexBuffer; }
	virtual ID3D11Buffer* GetIndexBuffer()const { return indexBuffer; }
	virtual int GetVertexStride()const { return sizeof(VertexPosColor); }
	virtual const char* GetShaderSetup()const { return "Model"; };
	virtual ~CModel()
	{
	};
};

class CQuad : public IModel
{
	static ID3D11Buffer* vertexBuffer;
	static ID3D11Buffer* indexBuffer;
	const char* shaderSetup;
public:
	bool Create(ID3D11Device* device);
	void SetShaderSetup(const char* setup) { shaderSetup=setup; }
	virtual ID3D11Buffer *const GetVertexBuffer()const { return vertexBuffer; }
	virtual ID3D11Buffer* GetIndexBuffer()const { return indexBuffer; }
	virtual int GetVertexStride()const { return sizeof(VertexPosTexUV); }
	virtual const char* GetShaderSetup()const { return shaderSetup; };
	virtual ~CQuad()
	{
	}
};
