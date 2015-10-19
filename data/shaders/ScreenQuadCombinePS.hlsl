Texture2D tex : register(t0);
SamplerState sam : register(s0);
Texture2D tex1 : register(t1);
SamplerState sam1 : register(s1);

cbuffer buf0 : register(b0)
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
	float4 c=0;
	float w=0;

	float2 uv=(IN.pos.xy)/2+0.5;
	float4 ub1=tex1.Sample(sam1,uv);
	float4 ub=tex.Sample(sam,uv);

	float2 uvscr=uv;
	uvscr.y=1-uvscr.y;
	uvscr*=scrsize.xy;
	float2 mp=mpos.xy;

	if(!mpos.z)
	{
		if(uvscr.x<scrsize.x/2)
			return ub;
		else
			return ub1;
	}
	else
	{
		if(length(uvscr-mp)>200)
			return ub;
		else
			return ub1;
	}

	return float4(c.rgb/w,1);
}