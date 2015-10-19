struct PixelShaderInput
{
    float4 color : COLOR;
	float3 pos : TEXCOORD0;
	float3 norm: TEXCOORD1;
};

float4 main( PixelShaderInput IN ) : SV_TARGET
{
	float3 uvw=frac(IN.pos*10)>0.5;
	float ch=(uvw.x+uvw.y)==1;
	ch=(ch+uvw.z)==1;
	float3 pos=IN.pos;
	pos*=30;
	ch=sin(pos.x)*sin(pos.y)*sin(pos.z);
	ch=ch*0.5+0.5;
	float lambert=dot(normalize(float3(1, 1, 1)), IN.norm)*0.5+0.5;
    return float4(lerp(float3(1.0,0.75,0.1),float3(0.0,0.5,1.0),ch)*lambert,1);
}