struct PixelShaderInput
{
    float4 color : COLOR;
};

float4 main( PixelShaderInput IN ) : SV_TARGET
{
	return float4(1, 0, 1, 1);
    return IN.color;
}