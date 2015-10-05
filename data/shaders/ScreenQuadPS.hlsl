cbuffer Xx : register(b0)
{
	float4 scrsize;
	float4 mpos;
}

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float4 pos : TEXCOORD0;
};

float4 main( PixelShaderInput IN ) : SV_TARGET0
{
	float2 uv=IN.pos.xy*scrsize.xy;
	uv=frac(uv/400)>0.5;
	float ch=(uv.x+uv.y)==1;

	return float4(1,ch,ch,1);
}