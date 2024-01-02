#include "Include/Common.hlsl"
struct PixelInput
{
    float4 Pos : SV_Position;
    float3 WorldPos : WorldPos;
};

TextureCube u_RadianceMap : register(t5);

float4 main(PixelInput Input) : SV_Target
{
    return u_RadianceMap.Sample(u_SSLinearWrap, Input.WorldPos);
}