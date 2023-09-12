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

Texture2D u_Textures[16] : register(t0);
SamplerState u_SamplerState[16] : register(s0);

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
        case 7:
            texColor = u_Textures[7].Sample(u_SamplerState[7], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 8:
            texColor = u_Textures[8].Sample(u_SamplerState[8], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 9:
            texColor = u_Textures[9].Sample(u_SamplerState[9], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 10:
            texColor = u_Textures[10].Sample(u_SamplerState[10], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 11:
            texColor = u_Textures[11].Sample(u_SamplerState[11], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 12:
            texColor = u_Textures[12].Sample(u_SamplerState[12], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 13:
            texColor = u_Textures[13].Sample(u_SamplerState[13], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 14:
            texColor = u_Textures[14].Sample(u_SamplerState[14], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
        case 15:
            texColor = u_Textures[15].Sample(u_SamplerState[15], Input.TexCoord * Input.TilingFactor) * Input.Color;
            break;
    }

    if (texColor.a == 0.0)
        discard;
	
    Output.Color = texColor;
    return Output;
}