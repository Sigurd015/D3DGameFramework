struct VertexInput
{
    float3 a_WorldPosition : a_WorldPosition;
    float3 a_LocalPosition : a_LocalPosition;
    float4 a_Color : a_Color;
    float a_Thickness : a_Thickness;
    float a_Fade : a_Fade;
};

struct VertexOutput
{
    float4 Pos : SV_Position;
    float3 LocalPosition : LoP;
    float4 Color : Cor;
    float Thickness : Ths;
    float Fade : Fde;
};

cbuffer Camera : register(b0)
{
    float4x4 u_ViewProjection;
};

VertexOutput main(VertexInput Input)
{
    VertexOutput Output;
    Output.LocalPosition = Input.a_LocalPosition;
    Output.Color = Input.a_Color;
    Output.Thickness = Input.a_Thickness;
    Output.Fade = Input.a_Fade;
    Output.Pos = mul(float4(Input.a_WorldPosition, 1.0f), u_ViewProjection);
    return Output;
}