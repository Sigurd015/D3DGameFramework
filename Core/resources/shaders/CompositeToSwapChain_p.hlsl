#include "Include/Common.hlsl"

Texture2D u_FinalImage : register(t0);

float4 main(float4 Position : SV_Position) : SV_Target
{
    return u_FinalImage.Load(uint3(Position.xy, 0)).rgba;
}