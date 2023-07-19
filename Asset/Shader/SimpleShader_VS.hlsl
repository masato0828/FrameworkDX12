#include "inc_SimpleShader.hlsli"

VSOutput main(float4 pos : SV_POSITION, float2 uv : TEXCOORD)
{
    VSOutput Out;
    Out.pos = pos;
    Out.uv = uv;
    
	return Out;
}