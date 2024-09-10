/*******************************************************************************
 *
 * rawfootprint solid shader
 *
 *******************************************************************************/

cbuffer ConstantBuffer : register(b0)
{
	matrix wvp : WorldViewProjection;
	float4 matColor = float4(0.8, 0.2, 0.0, 1.0);
}

void mainVS(
	float3 vertex : POSITION,
	float3 normal : NORMAL,
	out float4 position: SV_POSITION)
{ 
	position = mul(float4(vertex.xyz, 1.0f), wvp);
}

float4 mainPS() : SV_Target
{
	return matColor;
}
