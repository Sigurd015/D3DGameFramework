struct PixelInput
{
    float4 Pos : SV_Position;
    float3 LocalPosition : LoP;
    float4 Color : Cor;
    float Thickness : Ths;
    float Fade : Fde;
};

struct PixelOutput
{
    float4 Color : SV_Target0;
};

PixelOutput main(PixelInput Input)
{
    PixelOutput Output;
	
    // Calculate distance and fill circle with white
    float distance = 1.0f - length(Input.LocalPosition);
    float circle = smoothstep(0.0f, Input.Fade, distance);
    circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);
	
    if (circle == 0.0f)
        discard;
		
    Output.Color = Input.Color;
    Output.Color.a *= circle;
    return Output;
}