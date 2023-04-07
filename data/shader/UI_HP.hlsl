#include "ConstantHeader.txt"
struct VS_in
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};
struct VS_out
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float time : TEXCOORD1;
};

VS_out VS(VS_in input)
{
	VS_out output = (VS_out)0;

	output.p = float4(input.p, 1.0f);
	output.p = mul(output.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = input.n;
	output.c = input.c;
	output.t = input.t;
	output.time = g_fTimer;
	return output;
}

float4 PS(VS_out input) : SV_Target
{
	float4 vColor = g_txTexA.Sample(g_SampleWrap , input.t); 
	return vColor * input.c;
}

float4 COLOR_PS(VS_out input) : SV_Target
{
	return input.c;
}