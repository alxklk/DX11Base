Texture2D tex : register(t0);
SamplerState sam : register(s0);

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
	float4 c=0;
	float w=0;
	for(int i=0;i<=50;i++)
	{
		float x=i/50.0-0.5;
		float wi=exp(-x*x/25.0/25.0);
		c+=tex.Sample(sam,IN.pos.xy/2+float2(0,x*0.004))*wi;
		w+=wi;
	}
	return float4(c.rgb/w,1);
}