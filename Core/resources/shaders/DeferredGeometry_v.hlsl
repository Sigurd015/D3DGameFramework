#include "Include/Buffers.hlsl"

struct VertexInput
{
    float3 a_Position : a_Position;
    float3 a_Normal : a_Normal;
    float3 a_Tangent : a_Tangent;
    float3 a_Bitangent : a_Bitangent;
    float2 a_TexCoord : a_TexCoord;
};

struct VertexOutput
{
    float4 Position : SV_Position;
    float3 WorldPosition : WorldPos;
    float3 Normal : Nor;
    float2 TexCoord : Tex;
    float3x3 TBN : TBN;
};

VertexOutput main(VertexInput Input)
{
    VertexOutput Output;
    float4 worldPosition = mul(float4(Input.a_Position, 1.0f), u_Transform);
    Output.WorldPosition = worldPosition.xyz;
    Output.TexCoord = Input.a_TexCoord;
    Output.Normal = mul(Input.a_Normal, (float3x3) u_Transform);
    Output.TBN = mul(float3x3(Input.a_Tangent, Input.a_Bitangent, Input.a_Normal), (float3x3) u_Transform);
    Output.Position = mul(worldPosition, u_ViewProjection);
    return Output;
}