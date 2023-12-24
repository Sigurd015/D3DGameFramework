#include "Include/Common.hlsl"
#include "Include/Buffers.hlsl"

struct PixelInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TexCoord;
    float3 WorldPosition : WorldPosition;
};

Texture2D u_AlbedoTex : register(t0);

float main(PixelInput Input) : SV_Depth
{
    float4 albedo = u_AlbedoTex.Sample(u_SSAnisotropicWrap, Input.TexCoord);
    // Discard transparent pixels
    clip(albedo.a - 0.01f);

    // get distance between pixel and light source
    float lightDistance = length(Input.WorldPosition - u_PointLightPosition);

    // map to [0;1] range by dividing by far plane, then write this as modified depth
    return lightDistance / u_PointFarPlane;
}