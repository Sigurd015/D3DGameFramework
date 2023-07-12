#include "pch.h"
#include "Renderer2D.h"
#include "RendererAPI.h"

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
	Texture2D WhiteTexture;

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

		//PipelineSpecification pipelineSpec;
		//pipelineSpec.Layout = layout;
		//pipelineSpec.Shader = Renderer::GetShader("Renderer2D_Quad");
		//pipelineSpec.Topology = PrimitiveTopology::Triangles;

		//s_Data->QuadPipeline = Pipeline::Create(pipelineSpec);
		//s_Data->QuadMaterial = Material::Create(pipelineSpec.Shader);

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

		/*PipelineSpecification pipelineSpec;
		pipelineSpec.Layout = layout;
		pipelineSpec.Shader = Renderer::GetShader("Renderer2D_Circle");
		pipelineSpec.Topology = PrimitiveTopology::Triangles;

		s_Data->CirclePipeline = Pipeline::Create(pipelineSpec);
		s_Data->CircleMaterial = Material::Create(pipelineSpec.Shader);*/

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

		//PipelineSpecification pipelineSpec;
		//pipelineSpec.Layout = layout;
		//pipelineSpec.Shader = Renderer::GetShader("Renderer2D_Line");
		//pipelineSpec.Topology = PrimitiveTopology::Lines;

		//s_Data->LinePipeline = Pipeline::Create(pipelineSpec);
		//s_Data->LineMaterial = Material::Create(pipelineSpec.Shader);

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

		/*PipelineSpecification pipelineSpec;
		pipelineSpec.Layout = layout;
		pipelineSpec.Shader = Renderer::GetShader("Renderer2D_Text");
		pipelineSpec.Topology = PrimitiveTopology::Triangles;

		s_Data->TextPipeline = Pipeline::Create(pipelineSpec);
		s_Data->TextMaterial = Material::Create(pipelineSpec.Shader);*/

		s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];
	}

	// Set WhiteTexture slots to 0
	//s_Data->WhiteTexture = Renderer::GetTexture("White");
	//s_Data->TextureSlots[0] = s_Data->WhiteTexture;
	//
	ConstantBuffer_Create(&s_Data.CameraConstantBuffer, sizeof(Renderer2DData::CameraData), 0);
}

void Renderer2D_Shutdown()
{}

void Renderer2D_BeginScene()
{}

void Renderer2D_EndScene()
{}

void Renderer2D_DrawQuad(const Vec2* position, const Vec2* size, const Vec4* color)
{}
