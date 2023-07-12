struct VertexInput
{
    float3 a_Position : a_Position;
    float4 a_Color : a_Color;
    float2 a_TexCoord : a_TexCoord;
    int a_TexIndex : a_TexIndex;
    float a_TilingFactor : a_TilingFactor;
};

struct VertexOutput
{
    float4 Pos : SV_Position;
    float4 Color : Cor;
    float2 TexCoord : Ted;
    int TexIndex : Tei;
    float TilingFactor : Tlf;
};

cbuffer Camera : register(b0)
{
    float4x4 u_ViewProjection;
};

VertexOutput main(VertexInput Input)
{
    VertexOutput Output;
    Output.Color = Input.a_Color;
    Output.TexCoord = Input.a_TexCoord;
    Output.TexIndex = Input.a_TexIndex;
    Output.TilingFactor = Input.a_TilingFactor;
    Output.Pos = mul(u_ViewProjection, float4(Input.a_Position, 1.0f));
    return Output;
}