#include "stdafx.h"
#include "Model.h"


bool CModel::Create(ID3D11Device* device, VertexPosNormTexUV* vertices, int nVertices, WORD* indices, int nIndices)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth=sizeof(VertexPosNormTexUV) * nVertices;
	vertexBufferDesc.CPUAccessFlags=0;
	vertexBufferDesc.Usage=D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	memset(&resourceData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem=vertices;

	HRESULT hr=device->CreateBuffer(&vertexBufferDesc, &resourceData, &vertexBuffer);
	if(FAILED(hr))
	{
		return false;
	}

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth=sizeof(WORD) * nIndices;
	indexBufferDesc.CPUAccessFlags=0;
	indexBufferDesc.Usage=D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride=sizeof(WORD);
	resourceData.pSysMem=indices;

	indexCount=nIndices;


	hr=device->CreateBuffer(&indexBufferDesc, &resourceData, &indexBuffer);
	if(FAILED(hr))
	{
		return false;
	}

	return true;
}


bool CQuad::Create(ID3D11Device* device)
{
	VertexPosTexUV Vertices[4]=
	{
		{XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)}
	};

	WORD Indicies[6]=
	{
		0, 1, 2, 0, 2, 3
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth=sizeof(VertexPosTexUV)*4;
	vertexBufferDesc.CPUAccessFlags=0;
	vertexBufferDesc.Usage=D3D11_USAGE_DEFAULT;
	vertexBufferDesc.StructureByteStride=sizeof(VertexPosTexUV);

	D3D11_SUBRESOURCE_DATA resourceData;
	memset(&resourceData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem=Vertices;

	HRESULT hr=device->CreateBuffer(&vertexBufferDesc, &resourceData, &vertexBuffer);
	if(FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth=sizeof(WORD)*6;
	indexBufferDesc.CPUAccessFlags=0;
	indexBufferDesc.Usage=D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride=sizeof(WORD);
	resourceData.pSysMem=Indicies;

	hr=device->CreateBuffer(&indexBufferDesc, &resourceData, &indexBuffer);
	if(FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}
	return true;
}

ID3D11Buffer* CQuad::vertexBuffer=0;
ID3D11Buffer* CQuad::indexBuffer=0;
