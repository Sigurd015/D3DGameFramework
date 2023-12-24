#include "Include/Common.hlsl"

struct PixelInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TexCoord;
};

Texture2D u_AlbedoTex : register(t0);

void main(PixelInput Input)
{
    float4 albedo = u_AlbedoTex.Sample(u_SSAnisotropicWrap, Input.TexCoord);
    // Discard transparent pixels
    clip(albedo.a - 0.01f);
}