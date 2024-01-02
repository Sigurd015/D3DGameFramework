#include "Include/Buffers.hlsl"

struct VertexOutput
{
    float4 Pos : SV_Position;
    float3 WorldPos : WorldPos;
};

VertexOutput main(float3 Position : a_Position)
{
    VertexOutput Output;
    Output.Pos = mul(float4(Position, 0.0f), u_ViewProjection);
    Output.Pos.z = Output.Pos.w;
    Output.WorldPos = Position;
    return Output;
}