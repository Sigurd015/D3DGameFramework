#include "Include/Buffers.hlsl"
#include "Include/Common.hlsl"

struct PixelInput
{
    float4 Position : SV_Position;
    float3 WorldPosition : WorldPos;
    float3 Normal : Nor;
    float2 TexCoord : Tex;
    float3x3 TBN : TBN;
};

struct PixelOutput
{
    float4 Albedo : SV_Target0;
    float4 MRE : SV_Target1; // Metalness, Roughness, Emission
    float4 Normal : SV_Target2;
    float4 Position : SV_Target3;
};

Texture2D u_AlbedoTex : register(t0);
Texture2D u_MetallicRoughnessTex : register(t1);
Texture2D u_NormalTex : register(t2);

PixelOutput main(PixelInput Input)
{
    PixelOutput Output;
    float4 albedo = u_AlbedoTex.Sample(u_SSAnisotropicWrap, Input.TexCoord);
    // Notice: Use GLTF spec, Metalness is in B channel, Roughness is in G channel
    float4 metallicRoughness = u_MetallicRoughnessTex.Sample(u_SSLinearWrap, Input.TexCoord);
    float metalness = metallicRoughness.b * u_Material.Metalness;
    float roughness = metallicRoughness.g * u_Material.Roughness;

    float3 normal = normalize(Input.Normal);
    if (u_Material.UseNormalMap)
    {
        normal = normalize(u_NormalTex.Sample(u_SSLinearWrap, Input.TexCoord).rgb * 2.0f - 1.0f); // from RGB[0, 1] to [-1, 1]
        normal = normalize(mul(normal, Input.TBN));
    }

    // Convert albedo from sRGB to linear space
    albedo.rgb = SRGBToLinear(albedo.rgb, 2.2f);
    Output.Albedo = float4(albedo.rgb * u_Material.Albedo, albedo.a);
    // Minimum roughness of 0.05 to keep specular highlight
    Output.MRE = float4(metalness, max(roughness, 0.05f), u_Material.Emission, 1.0f);
    Output.Normal = float4(normal, 1.0f);
    Output.Position = float4(Input.WorldPosition, 1.0f);

    return Output;
}