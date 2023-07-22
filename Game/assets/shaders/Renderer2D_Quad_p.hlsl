struct PixelInput
{
    float4 Pos : SV_Position;
    float4 Color : Cor;
    float2 TexCoord : Ted;
    int TexIndex : Tei;
    float TilingFactor : Tlf;
};

struct PixelOutput
{
    float4 Color : SV_Target0;
};

Texture2D u_Textures[32] : register(t0);
SamplerState u_SamplerState[32] : register(s0);

PixelOutput main(PixelInput Input)
{
    PixelOutput Output;
    float4 texColor;
    switch (Input.TexIndex)
    {
        case 0:
            texColor = u_Textures[0].Sample(u_SamplerState[0], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 1:
            texColor = u_Textures[1].Sample(u_SamplerState[1], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 2:
            texColor = u_Textures[2].Sample(u_SamplerState[2], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 3:
            texColor = u_Textures[3].Sample(u_SamplerState[3], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 4:
            texColor = u_Textures[4].Sample(u_SamplerState[4], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 5:
            texColor = u_Textures[5].Sample(u_SamplerState[5], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 6:
            texColor = u_Textures[6].Sample(u_SamplerState[6], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
    }

    if (texColor.a == 0.0)
        discard;
	
    Output.Color = texColor;
    return Output;
}