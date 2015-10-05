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
	float2 uv=(IN.pos.xy*0.5+0.5);

	uv.y=1-uv.y;
	uv*=scrsize.xy;
	float2 chuv=frac(uv/100)>0.5;
	float ch=(chuv.x+chuv.y)==1;

	float2 mp=mpos.xy;

	return float4(1,ch,ch,1);
}