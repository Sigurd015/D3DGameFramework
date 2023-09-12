struct PixelInput
{
    float4 Pos : SV_Position;
    float4 Color : Cor;
};

struct PixelOutput
{
    float4 Color : SV_Target0;
};

PixelOutput main(PixelInput Input)
{
    PixelOutput Output;
    Output.Color = Input.Color;
    return Output;
}