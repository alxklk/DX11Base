#pragma once

#include "stdafx.h"

using namespace DirectX;
/*
struct VertexPosColor
{
	XMFLOAT3 pos;
	XMFLOAT3 col;
};
*/
struct VertexPosTexUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct VertexPosNormTexUV
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

class IModel
{
public:
	virtual ID3D11Buffer *const GetVertexBuffer()const=0;
	virtual ID3D11Buffer* GetIndexBuffer()const=0;
	virtual int GetIndexCount()const=0;
	virtual bool GetZEnabled()const=0;
	virtual int GetVertexStride()const=0;
	virtual const char* GetShaderSetup()const=0;
	virtual ID3D11ShaderResourceView* GetTextureView(int n)const=0;
	virtual ID3D11RenderTargetView* GetRTView()const=0;
	virtual void SetTextureView(int n, ID3D11ShaderResourceView*)=0;
	virtual void SetRTView(ID3D11RenderTargetView* )=0;
};

class CModel : public IModel
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	unsigned int  indexCount;
	ID3D11ShaderResourceView* textures[16];
	ID3D11RenderTargetView* RT;
public:
	bool Create(ID3D11Device* device, VertexPosNormTexUV* vertices, int nVertices, WORD* indices, int nIndices);
	CModel()
		: vertexBuffer(nullptr)
		, indexBuffer(nullptr)
		, indexCount(0)
		, RT(nullptr)
	{
		for(int i=0;i<16;i++)
			textures[i]=0;
	}
	virtual ID3D11Buffer *const GetVertexBuffer()const { return vertexBuffer; }
	virtual ID3D11Buffer* GetIndexBuffer()const { return indexBuffer; }
	virtual int GetIndexCount()const{return indexCount;};
	virtual int GetVertexStride()const { return sizeof(VertexPosNormTexUV); }
	virtual bool GetZEnabled()const { return true; };
	virtual const char* GetShaderSetup()const { return "model"; }
	virtual ID3D11ShaderResourceView* GetTextureView(int n)const {return textures[n];}
	virtual ID3D11RenderTargetView* GetRTView()const {return RT;}
	virtual void SetTextureView(int n, ID3D11ShaderResourceView* textureView){textures[n]=textureView;}
	virtual void SetRTView(ID3D11RenderTargetView*  RTView){RT=RTView;}
	virtual ~CModel()
	{
	};
};

class CQuad : public IModel
{
	static ID3D11Buffer* vertexBuffer;
	static ID3D11Buffer* indexBuffer;
	const char* shaderSetup;
	ID3D11ShaderResourceView* textures[16];
	ID3D11RenderTargetView* RT;
public:
	bool Create(ID3D11Device* device, float depth=0.0f);
	CQuad()
		: RT(nullptr)
	{
		for(int i=0;i<16;i++)
			textures[i]=0;
	}
	void SetShaderSetup(const char* setup) { shaderSetup=setup; }
	virtual ID3D11Buffer *const GetVertexBuffer()const { return vertexBuffer; }
	virtual ID3D11Buffer* GetIndexBuffer()const { return indexBuffer; }
	virtual int GetIndexCount()const{return 6;}
	virtual int GetVertexStride()const { return sizeof(VertexPosTexUV); }
	virtual bool GetZEnabled()const { return false; };
	virtual const char* GetShaderSetup()const { return shaderSetup; };
	virtual ID3D11ShaderResourceView* GetTextureView(int n)const {return textures[n];};
	virtual ID3D11RenderTargetView* GetRTView()const {return RT;};
	virtual void SetTextureView(int n, ID3D11ShaderResourceView* textureView){textures[n]=textureView;}
	virtual void SetRTView(ID3D11RenderTargetView*  RTView){RT=RTView;}
	virtual ~CQuad()
	{
	}
};
