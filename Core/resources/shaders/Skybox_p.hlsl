struct PixelInput
{
    float4 Pos : SV_Position;
    float3 WorldPos : WorldPos;
};

struct PixelOutput
{
    float4 Color : SV_Target0;
};

TextureCube u_RadianceMap : register(t5);
SamplerState u_SSLinearWrap : register(s0);

PixelOutput main(PixelInput Input)
{
    PixelOutput Output;
    Output.Color = u_RadianceMap.Sample(u_SSLinearWrap, Input.WorldPos);
    return Output;
}