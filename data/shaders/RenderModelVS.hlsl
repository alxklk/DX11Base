cbuffer buf0 : register(b0)
{
	float4x4 wm;
}

struct AppData
{
	float3 position : POSITION;
	float3 normal: NORMAL;
	float2 uv: TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 color : COLOR;
	float3 pos : TEXCOORD0;
	float4 position : SV_POSITION;
};

VertexShaderOutput main(AppData IN)
{
	VertexShaderOutput OUT;
	OUT.position = float4(IN.position.xy, 0.5, 1.0f);
//	OUT.position = mul(wm, float4(IN.position, 1.0f));
	OUT.position.x += wm._01;
	OUT.color=float4(IN.position.yz, IN.position.x*0.1, 1.0f);
	OUT.pos=IN.position.xyz;
	return OUT;

//	matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
//	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.color = float4(IN.uv, 0.0, 1.0f);

	return OUT;
}