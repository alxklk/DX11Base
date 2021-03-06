struct AppData
{
	float3 position : POSITION;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float4 pos : TEXCOORD0;
};

VertexShaderOutput main(AppData IN)
{
	VertexShaderOutput OUT;
	OUT.position = float4(IN.position, 1.0f);
	OUT.pos= float4(IN.position.xyz, 1.0f);
	return OUT;
}