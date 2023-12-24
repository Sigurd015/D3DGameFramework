#include "Include/Buffers.hlsl"

struct VertexInput
{
    float3 a_Position : a_Position;
};

struct VertexOutput
{
    float4 Pos : SV_Position;
    float3 WorldPos : WorldPos;
};

VertexOutput main(VertexInput Input)
{
    VertexOutput Output;
    Output.Pos = mul(u_ViewProjection, float4(Input.a_Position, 0.0f));
    Output.Pos.z = Output.Pos.w;
    Output.WorldPos = Input.a_Position;
    return Output;
}