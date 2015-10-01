/*cbuffer PerApplication : register(b0)
{
	matrix projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
	matrix viewMatrix;
}

cbuffer PerObject : register(b2)
{
	matrix worldMatrix;
}
*/
struct AppData
{
	float3 position : POSITION;
	float3 normal: NORMAL;
	float2 uv: TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

VertexShaderOutput main(AppData IN)
{
	VertexShaderOutput OUT;
	OUT.position = float4(IN.position.xy, 0.5, 1.0f);
	OUT.color=float4(IN.position.yz, 0.0, 1.0f);
	return OUT;

//	matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
//	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.color = float4(IN.uv, 0.0, 1.0f);

	return OUT;
}