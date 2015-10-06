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

	float2 uv=(IN.pos.xy)/2+0.5;
	float4 ub=tex.Sample(sam,uv);
//	return float4(uv,0,1);
//	return ub;

	float2 uvscr=uv;
	uvscr.y=1-uvscr.y;
	uvscr*=scrsize.xy;
	float2 mp=mpos.xy;

	for(int i=-25;i<=25;i++)
	{
		float x=i/25.0;
		float wi=exp(-x*x);
		c+=tex.Sample(sam,IN.pos.xy/2-0.5+float2(0,x*0.04))*wi;
		w+=wi;
	}

	if(length(uvscr-mp)>200)
		return ub;


	return float4(c.rgb/w,1);
}