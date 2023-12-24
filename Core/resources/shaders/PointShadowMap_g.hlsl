#include "Include/Buffers.hlsl"

struct GSInput
{
    float4 Vertex : SV_Position;
    float2 TexCoord : TexCoord;
};

struct GSOutput
{
    float4 Vertex : SV_Position;
    float2 TexCoord : TexCoord;
    float3 WorldPosition : WorldPosition;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(18)] //6 * 3 = 18
void main(
	triangle GSInput Input[3], //Takes the output from vertex shader * 3
	inout TriangleStream<GSOutput> OutputStream //Sends to pixel shader
)
{
    for (uint CubeFaceIndex = 0; CubeFaceIndex < 6; CubeFaceIndex++)
    {
        GSOutput output;
        output.RTIndex = CubeFaceIndex; //Keep track of cube face
        
        //Triangle = 3 vertices
        for (int VertexIndex = 0; VertexIndex < 3; VertexIndex++)
        {
            output.WorldPosition = Input[VertexIndex].Vertex.xyz;
            output.TexCoord = Input[VertexIndex].TexCoord;
            output.Vertex = mul(u_PointLightViewProj[CubeFaceIndex], float4(Input[VertexIndex].Vertex.xyz, 1.0f));
            OutputStream.Append(output);
        }
        OutputStream.RestartStrip();
    }
}