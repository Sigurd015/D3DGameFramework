struct VertexInput
{
    float3 a_Position : a_Position;
    float4 a_Color : a_Color;
};

struct VertexOutput
{
    float4 Pos : SV_Position;
    float4 Color : Cor;
};

cbuffer Camera : register(b0)
{
    float4x4 u_ViewProjection;
};

VertexOutput main(VertexInput Input)
{
    VertexOutput Output;
    Output.Color = Input.a_Color;
    Output.Pos = mul(float4(Input.a_Position, 1.0f), u_ViewProjection);
    return Output;
}