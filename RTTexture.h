#pragma once

#include <d3d11.h>

class CRTTexture
{
	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11ShaderResourceView* shaderResourceView;
public:
	CRTTexture(void);
	bool Create(ID3D11Device* device, int width, int height, DXGI_FORMAT format);
	bool Destroy();
	ID3D11ShaderResourceView* GetShaderResourceView(){return shaderResourceView;}
	ID3D11RenderTargetView* GetRTView(){return renderTargetView;}
	~CRTTexture(void);
};

