#include "pch.h"
#include "Renderer2D.h"
#include "RendererAPI.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Pipeline.h"
#include "ConstantBuffer.h"
#include "Texture.h"

struct QuadVertex
{
	Vec3 Position;
	Vec4 Color;
	Vec2 TexCoord;
	int TexIndex;
	float TilingFactor;
};

struct CircleVertex
{
	Vec3 WorldPosition;
	Vec3 LocalPosition;
	Vec4 Color;
	float Thickness;
	float Fade;
};

struct LineVertex
{
	Vec3 Position;
	Vec4 Color;
};

struct TextVertex
{
	Vec3 Position;
	Vec4 Color;
	Vec2 TexCoord;
};

struct Renderer2DData
{
	static const uint32_t MaxQuads = 10000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const uint32_t MaxTextureSlots = 32;

	Pipeline QuadPipeline;
	VertexBuffer QuadVertexBuffer;
	IndexBuffer QuadIndexBuffer;
	uint32_t QuadIndexCount = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;

	Pipeline CirclePipeline;
	VertexBuffer CircleVertexBuffer;
	IndexBuffer CircleIndexBuffer;
	uint32_t CircleIndexCount = 0;
	CircleVertex* CircleVertexBufferBase = nullptr;
	CircleVertex* CircleVertexBufferPtr = nullptr;

	Pipeline LinePipeline;
	VertexBuffer LineVertexBuffer;
	uint32_t LineVertexCount = 0;
	LineVertex* LineVertexBufferBase = nullptr;
	LineVertex* LineVertexBufferPtr = nullptr;
	float LineWidth = 2.0f;

	Pipeline TextPipeline;
	VertexBuffer TextVertexBuffer;
	IndexBuffer TextIndexBuffer;
	uint32_t TextIndexCount = 0;
	TextVertex* TextVertexBufferBase = nullptr;
	TextVertex* TextVertexBufferPtr = nullptr;

	Texture2D Textures[32];// 0 is white texture
	uint32_t TextureSlotIndex = 1;
	Texture2D FontAtlasTexture;

	Vec4 QuadVertexPositions[4] =
	{ { -0.5f, -0.5f, 0.0f, 1.0f },
	  {  0.5f, -0.5f, 0.0f, 1.0f },
	  {  0.5f,  0.5f, 0.0f, 1.0f },
	  { -0.5f,  0.5f, 0.0f, 1.0f } };
	Vec2 QuadTexCoord[4] =
	{ { 0.0f, 0.0f },
	  { 1.0f, 0.0f },
	  { 1.0f, 1.0f },
	  { 0.0f, 1.0f } };

	struct CameraData
	{
		Mat4 ViewProjection;
	};
	CameraData SceneBuffer;
	ConstantBuffer CameraConstantBuffer;
};
static Renderer2DData s_Data;

void Renderer2D_Initialize()
{
	//Quad
	{
		VertexBufferLayoutEmelent layoutEmelent[5] = {
		   { ShaderDataType::Float3, "a_Position"     },
		   { ShaderDataType::Float4, "a_Color"        },
		   { ShaderDataType::Float2, "a_TexCoord"     },
		   { ShaderDataType::Int,    "a_TexIndex"     },
		   { ShaderDataType::Float,  "a_TilingFactor" },
		};
		VertexBufferLayout layout = { layoutEmelent, 5 };
		VertexBufferLayout_CalculateOffsetsAndStride(&layout);
		VertexBuffer_Create(&s_Data.QuadVertexBuffer, s_Data.MaxVertices * sizeof(QuadVertex));
		VertexBuffer_SetLayout(&s_Data.QuadVertexBuffer, &layout);

		uint32_t* indices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 2;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 0;

			indices[i + 3] = offset + 0;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 2;

			offset += 4;
		}
		IndexBuffer_Create(&s_Data.QuadIndexBuffer, indices, s_Data.MaxIndices);
		delete[] indices;

		Shader shader;
		Shader_Create(&shader, "Renderer2D_Quad");

		PipelineSpecification spec;
		spec.Layout = &layout;
		spec.Shader = &shader;
		Pipeline_Create(&s_Data.QuadPipeline, &spec);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
	}

	// Circles
	{
		VertexBufferLayoutEmelent layoutEmelent[5] = {
		  { ShaderDataType::Float3, "a_WorldPosition" },
		  { ShaderDataType::Float3, "a_LocalPosition" },
		  { ShaderDataType::Float4, "a_Color"         },
		  { ShaderDataType::Float,  "a_Thickness"     },
		  { ShaderDataType::Float,  "a_Fade"          },
		};
		VertexBufferLayout layout = { layoutEmelent, 5 };
		VertexBufferLayout_CalculateOffsetsAndStride(&layout);
		VertexBuffer_Create(&s_Data.CircleVertexBuffer, s_Data.MaxVertices * sizeof(CircleVertex));
		VertexBuffer_SetLayout(&s_Data.CircleVertexBuffer, &layout);

		s_Data.CircleIndexBuffer = s_Data.QuadIndexBuffer;

		Shader shader;
		Shader_Create(&shader, "Renderer2D_Circle");

		PipelineSpecification spec;
		spec.Layout = &layout;
		spec.Shader = &shader;
		Pipeline_Create(&s_Data.CirclePipeline, &spec);

		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];
	}

	// Lines
	{
		VertexBufferLayoutEmelent layoutEmelent[2] = {
		  { ShaderDataType::Float3, "a_Position" },
		  { ShaderDataType::Float4, "a_Color"    },
		};
		VertexBufferLayout layout = { layoutEmelent, 2 };
		VertexBufferLayout_CalculateOffsetsAndStride(&layout);
		VertexBuffer_Create(&s_Data.LineVertexBuffer, s_Data.MaxVertices * sizeof(LineVertex));
		VertexBuffer_SetLayout(&s_Data.LineVertexBuffer, &layout);

		Shader shader;
		Shader_Create(&shader, "Renderer2D_Line");

		PipelineSpecification spec;
		spec.Layout = &layout;
		spec.Shader = &shader;
		Pipeline_Create(&s_Data.LinePipeline, &spec);

		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];
	}

	// Text
	{
		VertexBufferLayoutEmelent layoutEmelent[3] = {
		  { ShaderDataType::Float3, "a_Position"     },
		  { ShaderDataType::Float4, "a_Color"        },
		  { ShaderDataType::Float2, "a_TexCoord"     },
		};
		VertexBufferLayout layout = { layoutEmelent, 3 };
		VertexBufferLayout_CalculateOffsetsAndStride(&layout);
		VertexBuffer_Create(&s_Data.TextVertexBuffer, s_Data.MaxVertices * sizeof(TextVertex));
		VertexBuffer_SetLayout(&s_Data.TextVertexBuffer, &layout);

		uint32_t* indices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}
		IndexBuffer_Create(&s_Data.TextIndexBuffer, indices, s_Data.MaxIndices);
		delete[] indices;

		Shader shader;
		Shader_Create(&shader, "Renderer2D_Text");

		PipelineSpecification spec;
		spec.Layout = &layout;
		spec.Shader = &shader;
		Pipeline_Create(&s_Data.TextPipeline, &spec);

		s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];
	}

	// Set WhiteTexture slots to 0
	TextureSpecification spec;
	spec.Width = 1;
	spec.Height = 1;
	spec.Format = ImageFormat::RGBA8;

	Texture2D_Create(&s_Data.Textures[0], &spec);
	uint32_t whiteTextureData = 0xffffffff;
	Texture2D_SetData(&s_Data.Textures[0], &whiteTextureData, sizeof(uint32_t));

	ConstantBuffer_Create(&s_Data.CameraConstantBuffer, sizeof(Renderer2DData::CameraData), 0);
}

void Renderer2D_Shutdown()
{
	VertexBuffer_Release(&s_Data.QuadVertexBuffer);
	VertexBuffer_Release(&s_Data.CircleVertexBuffer);
	VertexBuffer_Release(&s_Data.LineVertexBuffer);
	VertexBuffer_Release(&s_Data.TextVertexBuffer);

	IndexBuffer_Release(&s_Data.QuadIndexBuffer);
	IndexBuffer_Release(&s_Data.CircleIndexBuffer);
	IndexBuffer_Release(&s_Data.TextIndexBuffer);

	Pipeline_Release(&s_Data.QuadPipeline);
	Pipeline_Release(&s_Data.CirclePipeline);
	Pipeline_Release(&s_Data.LinePipeline);

	ConstantBuffer_Release(&s_Data.CameraConstantBuffer);

	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		Texture2D_Release(&s_Data.Textures[i]);

	delete[] s_Data.QuadVertexBufferBase;
	delete[] s_Data.CircleVertexBufferBase;
	delete[] s_Data.LineVertexBufferBase;
	delete[] s_Data.TextVertexBufferBase;
}

void Renderer2D_BeginScene()
{}

void Renderer2D_EndScene()
{}

void Renderer2D_DrawQuad()
{}