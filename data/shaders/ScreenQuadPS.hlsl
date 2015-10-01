struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float4 pos : TEXCOORD0;
};

float4 main( PixelShaderInput IN ) : SV_TARGET0
{
	return float4(frac(IN.pos.xy*4)>0.5,0, 1);
}