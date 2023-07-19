#include "pch.h"
#include "Renderer2D.h"
#include "RendererAPI.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Pipeline.h"
#include "ConstantBuffer.h"
#include "Texture.h"

enum CBBingSlot
{
	CAMERA = 0,
};

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
		Mat ViewProjection;
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
		VertexBufferLayout_CalculateOffsetsAndStride(layout);
		VertexBuffer_Create(s_Data.QuadVertexBuffer, s_Data.MaxVertices * sizeof(QuadVertex));
		VertexBuffer_SetLayout(s_Data.QuadVertexBuffer, layout);

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
		IndexBuffer_Create(s_Data.QuadIndexBuffer, indices, s_Data.MaxIndices);
		delete[] indices;

		Shader shader;
		Shader_Create(shader, "Renderer2D_Quad");

		PipelineSpecification spec;
		spec.Layout = &layout;
		spec.Shader = &shader;
		Pipeline_Create(s_Data.QuadPipeline, spec);

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
		VertexBufferLayout_CalculateOffsetsAndStride(layout);
		VertexBuffer_Create(s_Data.CircleVertexBuffer, s_Data.MaxVertices * sizeof(CircleVertex));
		VertexBuffer_SetLayout(s_Data.CircleVertexBuffer, layout);

		s_Data.CircleIndexBuffer = s_Data.QuadIndexBuffer;

		Shader shader;
		Shader_Create(shader, "Renderer2D_Circle");

		PipelineSpecification spec;
		spec.Layout = &layout;
		spec.Shader = &shader;
		Pipeline_Create(s_Data.CirclePipeline, spec);

		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];
	}

	// Lines
	{
		VertexBufferLayoutEmelent layoutEmelent[2] = {
		  { ShaderDataType::Float3, "a_Position" },
		  { ShaderDataType::Float4, "a_Color"    },
		};
		VertexBufferLayout layout = { layoutEmelent, 2 };
		VertexBufferLayout_CalculateOffsetsAndStride(layout);
		VertexBuffer_Create(s_Data.LineVertexBuffer, s_Data.MaxVertices * sizeof(LineVertex));
		VertexBuffer_SetLayout(s_Data.LineVertexBuffer, layout);

		Shader shader;
		Shader_Create(shader, "Renderer2D_Line");

		PipelineSpecification spec;
		spec.Layout = &layout;
		spec.Shader = &shader;
		Pipeline_Create(s_Data.LinePipeline, spec);

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
		VertexBufferLayout_CalculateOffsetsAndStride(layout);
		VertexBuffer_Create(s_Data.TextVertexBuffer, s_Data.MaxVertices * sizeof(TextVertex));
		VertexBuffer_SetLayout(s_Data.TextVertexBuffer, layout);

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
		IndexBuffer_Create(s_Data.TextIndexBuffer, indices, s_Data.MaxIndices);
		delete[] indices;

		Shader shader;
		Shader_Create(shader, "Renderer2D_Text");

		PipelineSpecification spec;
		spec.Layout = &layout;
		spec.Shader = &shader;
		Pipeline_Create(s_Data.TextPipeline, spec);

		s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];
	}

	// Set WhiteTexture slots to 0
	TextureSpecification spec;
	spec.Width = 1;
	spec.Height = 1;
	spec.Format = ImageFormat::RGBA8;

	Texture2D_Create(s_Data.Textures[0], spec);
	uint32_t whiteTextureData = 0xffffffff;
	Texture2D_SetData(s_Data.Textures[0], &whiteTextureData, sizeof(uint32_t));

	ConstantBuffer_Create(s_Data.CameraConstantBuffer, sizeof(Renderer2DData::CameraData), CBBingSlot::CAMERA);
}

void Renderer2D_Shutdown()
{
	VertexBuffer_Release(s_Data.QuadVertexBuffer);
	VertexBuffer_Release(s_Data.CircleVertexBuffer);
	VertexBuffer_Release(s_Data.LineVertexBuffer);
	VertexBuffer_Release(s_Data.TextVertexBuffer);

	IndexBuffer_Release(s_Data.QuadIndexBuffer);
	IndexBuffer_Release(s_Data.CircleIndexBuffer);
	IndexBuffer_Release(s_Data.TextIndexBuffer);

	Pipeline_Release(s_Data.QuadPipeline);
	Pipeline_Release(s_Data.CirclePipeline);
	Pipeline_Release(s_Data.LinePipeline);

	ConstantBuffer_Release(s_Data.CameraConstantBuffer);

	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		Texture2D_Release(s_Data.Textures[i]);

	delete[] s_Data.QuadVertexBufferBase;
	delete[] s_Data.CircleVertexBufferBase;
	delete[] s_Data.LineVertexBufferBase;
	delete[] s_Data.TextVertexBufferBase;
}

void StartBatch()
{
	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	s_Data.CircleIndexCount = 0;
	s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

	s_Data.LineVertexCount = 0;
	s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

	s_Data.TextIndexCount = 0;
	s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;

	s_Data.TextureSlotIndex = 1;
}

void Renderer2D_BeginScene(const Mat& viewProjection)
{
	s_Data.SceneBuffer.ViewProjection = viewProjection;

	ConstantBuffer_SetData(s_Data.CameraConstantBuffer, &s_Data.SceneBuffer);

	Pipeline_SetConstantBuffer(s_Data.QuadPipeline, s_Data.CameraConstantBuffer);
	Pipeline_SetConstantBuffer(s_Data.CirclePipeline, s_Data.CameraConstantBuffer);
	Pipeline_SetConstantBuffer(s_Data.LinePipeline, s_Data.CameraConstantBuffer);
	Pipeline_SetConstantBuffer(s_Data.TextPipeline, s_Data.CameraConstantBuffer);

	StartBatch();
}

void Flush()
{
	if (s_Data.QuadIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		VertexBuffer_SetData(s_Data.QuadVertexBuffer, s_Data.QuadVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			Texture2D_Bind(s_Data.Textures[i], i);

		RendererAPI_DrawIndexed(s_Data.QuadVertexBuffer, s_Data.QuadIndexBuffer, s_Data.QuadPipeline, s_Data.QuadIndexCount);
	}

	if (s_Data.CircleIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
		VertexBuffer_SetData(s_Data.CircleVertexBuffer, s_Data.CircleVertexBufferBase, dataSize);

		// Use quad QuadIndexBuffer
		RendererAPI_DrawIndexed(s_Data.CircleVertexBuffer, s_Data.CircleIndexBuffer, s_Data.CirclePipeline, s_Data.CircleIndexCount);
	}

	if (s_Data.LineVertexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
		VertexBuffer_SetData(s_Data.LineVertexBuffer, s_Data.LineVertexBufferBase, dataSize);

		RendererAPI_DrawLines(s_Data.LineVertexBuffer, s_Data.LinePipeline, s_Data.LineVertexCount);
	}

	if (s_Data.TextIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TextVertexBufferPtr - (uint8_t*)s_Data.TextVertexBufferBase);
		VertexBuffer_SetData(s_Data.TextVertexBuffer, s_Data.TextVertexBufferBase, dataSize);

		// Bind textures
		Texture2D_Bind(s_Data.FontAtlasTexture, 0);

		RendererAPI_DrawIndexed(s_Data.TextVertexBuffer, s_Data.TextIndexBuffer, s_Data.TextPipeline, s_Data.TextIndexCount);
	}
}

void Renderer2D_EndScene()
{
	Flush();
}

void NextBatch()
{
	Flush();
	StartBatch();
}

void SetQuadVertex(const Mat& transform,
	const Vec4& color, const Vec2* texCoord, float texIndex, float tilingFactor)
{
	for (size_t i = 0; i < 4; i++)
	{
		s_Data.QuadVertexBufferPtr->Position = Vec4ToVec3(Vec4MulMat(s_Data.QuadVertexPositions[i], transform));
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoord[i];
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;
	}
	s_Data.QuadIndexCount += 6;
}

void Renderer2D_DrawQuad(const Mat& transform, const Vec4& color)
{
	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		NextBatch();

	const float texIndex = 0.0f; // White Texture
	const float tilingFactor = 1.0f;

	SetQuadVertex(transform, color, s_Data.QuadTexCoord, texIndex, tilingFactor);
}

float GetTextureID(const Texture2D& texture)
{
	float texIndex = 0.0f;
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
	{
		if (Texture2D_IsSame(s_Data.Textures[i], texture))
		{
			texIndex = (float)i;
			break;
		}
	}

	if (texIndex == 0.0f)
	{
		if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			NextBatch();

		texIndex = (float)s_Data.TextureSlotIndex;
		s_Data.Textures[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;
	}
	return texIndex;
}

void Renderer2D_DrawQuad(const Mat& transform, const Texture2D& texture, Vec2 uv0, Vec2 uv1, const Vec4& tintColor, float tilingFactor)
{
	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		NextBatch();

	Vec2 textureCoords[] = { uv0, { uv1.x, uv0.y }, uv1, { uv0.x, uv1.y } };
	SetQuadVertex(transform, tintColor, textureCoords, GetTextureID(texture), tilingFactor);
}

void Renderer2D_DrawCircle(const Mat& transform, const Vec4& color, float thickness, float fade)
{
	if (s_Data.CircleIndexCount >= Renderer2DData::MaxIndices)
		NextBatch();

	for (size_t i = 0; i < 4; i++)
	{
		s_Data.CircleVertexBufferPtr->WorldPosition = Vec4ToVec3(Vec4MulMat(s_Data.QuadVertexPositions[i], transform));
		s_Data.CircleVertexBufferPtr->LocalPosition = Vec4ToVec3(Vec4MulFloat(s_Data.QuadVertexPositions[i], 2.0f));
		s_Data.CircleVertexBufferPtr->Color = color;
		s_Data.CircleVertexBufferPtr->Thickness = thickness;
		s_Data.CircleVertexBufferPtr->Fade = fade;
		s_Data.CircleVertexBufferPtr++;
	}

	s_Data.CircleIndexCount += 6;
}

void Renderer2D_DrawLine(const Vec3& p0, Vec3& p1, const Vec4& color)
{
	s_Data.LineVertexBufferPtr->Position = p0;
	s_Data.LineVertexBufferPtr->Color = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexBufferPtr->Position = p1;
	s_Data.LineVertexBufferPtr->Color = color;
	s_Data.LineVertexBufferPtr++;

	s_Data.LineVertexCount += 2;
}

void Renderer2D_DrawRect(const Vec3& position, const Vec2& size, const Vec4& color)
{
	Vec3 p0 = Vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
	Vec3 p1 = Vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
	Vec3 p2 = Vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
	Vec3 p3 = Vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

	Renderer2D_DrawLine(p0, p1, color);
	Renderer2D_DrawLine(p1, p2, color);
	Renderer2D_DrawLine(p2, p3, color);
	Renderer2D_DrawLine(p3, p0, color);
}

void Renderer2D_DrawRect(const Mat& transform, const Vec4& color)
{
	Vec3 lineVertices[4];
	for (size_t i = 0; i < 4; i++)
		lineVertices[i] = Vec4ToVec3(Vec4MulMat(s_Data.QuadVertexPositions[i], transform));

	Renderer2D_DrawLine(lineVertices[0], lineVertices[1], color);
	Renderer2D_DrawLine(lineVertices[1], lineVertices[2], color);
	Renderer2D_DrawLine(lineVertices[2], lineVertices[3], color);
	Renderer2D_DrawLine(lineVertices[3], lineVertices[0], color);
}