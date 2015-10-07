Texture2D tex : register(t0);
SamplerState sam : register(s0);

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

	for(int i=-25;i<=25;i++)
	{
		float x=i/25.0;
		float wi=exp(-x*x);
		c+=tex.Sample(sam,IN.pos.xy/2-0.5+float2(x*0.03,0))*wi;
		w+=wi;
	}

	return float4(c.rgb/w,1);
}