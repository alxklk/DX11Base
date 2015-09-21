#include "stdafx.h"
#include "Model.h"


bool CModel::Create(ID3D11Device* device, VertexPosColor* vertices, int nVertices, WORD* indices, int nIndices)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosColor) * _countof(Vertices);
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resourceData;
	memset(&resourceData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem = Vertices;

	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &resourceData, &model.vertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(WORD) * _countof(Indicies);
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride = sizeof(WORD);
	resourceData.pSysMem = Indicies;

	hr = device->CreateBuffer(&indexBufferDesc, &resourceData, &model.indexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_BUFFER_DESC constantBufferDesc;
	memset(&constantBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = device->CreateBuffer(&constantBufferDesc, nullptr, &model.constantBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	ID3DBlob* psBlob = 0;
	ID3DBlob* vsBlob = 0;
	model.vertexShader = LoadVertexShader(appState, L"..\\data\\shaders\\RenderModelVS.hlsl", "main", "vs_4_0", &vsBlob);
	model.pixelShader = LoadPixelShader(appState, L"..\\data\\shaders\\RenderModelPS.hlsl", "main", "ps_4_0");

	// Create the input layout for the vertex shader.
	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, col), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &model.inputLayout);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}


bool CQuad::Create()
{
	ID3D11Device* device = g_AppState.GetRenderer()->GetDevice();

	VertexPosTexUV Vertices[4] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) }
	};

	WORD Indicies[6] =
	{
		0, 1, 2, 0, 2, 3
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexPosColor) * 4;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.StructureByteStride = sizeof(VertexPosColor);

	D3D11_SUBRESOURCE_DATA resourceData;
	memset(&resourceData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	resourceData.pSysMem = Vertices;

	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &resourceData, &model.vertexBuffer);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(WORD) * 6;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride = sizeof(WORD);
	resourceData.pSysMem = Indicies;

	hr = device->CreateBuffer(&indexBufferDesc, &resourceData, &model.indexBuffer);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	D3D11_BUFFER_DESC constantBufferDesc;
	memset(&constantBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = device->CreateBuffer(&constantBufferDesc, nullptr, &model.constantBuffer);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	ID3DBlob* vsBlob = 0;
	model.vertexShader = LoadVertexShader(appState, L"..\\data\\shaders\\ScreenQuadVS.hlsl", "main", "vs_5_0", &vsBlob);
	model.pixelShader = LoadPixelShader(appState, L"..\\data\\shaders\\ScreenQuadPS.hlsl", "main", "ps_5_0");

	int posoffset = offsetof(VertexPosTexUV, pos);
	int uvoffset = offsetof(VertexPosTexUV, uv);

	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, posoffset, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, uvoffset, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &model.inputLayout);
	if (FAILED(hr))
	{
		printf("Fail\n");
		return false;
	}

	return true;
}

