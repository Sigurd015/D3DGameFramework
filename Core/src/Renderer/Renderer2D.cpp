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

struct TextRenderCommand
{
	Vec2 Position;
	Vec4 Color;
	float FontSize;
	const WCHAR* FontFamilyName;
	const WCHAR* Text;
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
	uint32_t CircleIndexCount = 0;
	CircleVertex* CircleVertexBufferBase = nullptr;
	CircleVertex* CircleVertexBufferPtr = nullptr;

	Pipeline LinePipeline;
	VertexBuffer LineVertexBuffer;
	uint32_t LineVertexCount = 0;
	LineVertex* LineVertexBufferBase = nullptr;
	LineVertex* LineVertexBufferPtr = nullptr;
	float LineWidth = 2.0f;

	Pipeline UIPipeline;
	VertexBuffer UIVertexBuffer;
	uint32_t UIIndexCount = 0;
	QuadVertex* UIVertexBufferBase = nullptr;
	QuadVertex* UIVertexBufferPtr = nullptr;

	List TextRenderCommands;
	uint32_t TextRenderCommandCount = 0;

	Texture2D* Textures[32];// 0 is white texture
	Texture2D WhiteTexture;
	uint32_t TextureSlotIndex = 1;

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
	ConstantBuffer IdentityConstantBuffer;
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

	// UI
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
		VertexBuffer_Create(s_Data.UIVertexBuffer, s_Data.MaxVertices * sizeof(TextVertex));
		VertexBuffer_SetLayout(s_Data.UIVertexBuffer, layout);

		// Using the same shader as quads, but binding a identity viewProjection matrix.
		// Because UI position is already in screen space.
		Shader shader;
		Shader_Create(shader, "Renderer2D_Quad");

		PipelineSpecification spec;
		spec.Layout = &layout;
		spec.Shader = &shader;
		Pipeline_Create(s_Data.UIPipeline, spec);

		s_Data.UIVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
	}

	// Set WhiteTexture slots to 0
	TextureSpecification spec;
	spec.Width = 1;
	spec.Height = 1;
	spec.Format = ImageFormat::RGBA8;

	Texture2D_Create(s_Data.WhiteTexture, spec);
	uint32_t whiteTextureData = 0xffffffff;
	Texture2D_SetData(s_Data.WhiteTexture, &whiteTextureData, sizeof(uint32_t));
	s_Data.Textures[0] = &s_Data.WhiteTexture;

	ConstantBuffer_Create(s_Data.CameraConstantBuffer, sizeof(Renderer2DData::CameraData), CBBingSlot::CAMERA);
	ConstantBuffer_Create(s_Data.IdentityConstantBuffer, sizeof(Renderer2DData::CameraData), CBBingSlot::CAMERA);

	static Mat identity = DirectX::XMMatrixIdentity();
	ConstantBuffer_SetData(s_Data.IdentityConstantBuffer, &identity);
	Pipeline_SetConstantBuffer(s_Data.UIPipeline, s_Data.IdentityConstantBuffer);

	Pipeline_SetConstantBuffer(s_Data.QuadPipeline, s_Data.CameraConstantBuffer);
	Pipeline_SetConstantBuffer(s_Data.CirclePipeline, s_Data.CameraConstantBuffer);
	Pipeline_SetConstantBuffer(s_Data.LinePipeline, s_Data.CameraConstantBuffer);

	List_Create(s_Data.TextRenderCommands, COMMAND_BUFFER_SIZE);
	{
		TextRenderCommand command;
		for (size_t i = 0; i < COMMAND_BUFFER_SIZE; i++)
		{
			List_Add(s_Data.TextRenderCommands, sizeof(TextRenderCommand), &command);
		}
	}
}

void Renderer2D_Shutdown()
{
	VertexBuffer_Release(s_Data.QuadVertexBuffer);
	VertexBuffer_Release(s_Data.CircleVertexBuffer);
	VertexBuffer_Release(s_Data.LineVertexBuffer);
	VertexBuffer_Release(s_Data.UIVertexBuffer);

	IndexBuffer_Release(s_Data.QuadIndexBuffer);

	Pipeline_Release(s_Data.QuadPipeline);
	Pipeline_Release(s_Data.CirclePipeline);
	Pipeline_Release(s_Data.LinePipeline);
	Pipeline_Release(s_Data.UIPipeline);

	ConstantBuffer_Release(s_Data.CameraConstantBuffer);
	ConstantBuffer_Release(s_Data.IdentityConstantBuffer);

	List_Free(s_Data.TextRenderCommands, true);

	delete[] s_Data.QuadVertexBufferBase;
	delete[] s_Data.CircleVertexBufferBase;
	delete[] s_Data.LineVertexBufferBase;
	delete[] s_Data.UIVertexBufferBase;
}

void StartBatch()
{
	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	s_Data.CircleIndexCount = 0;
	s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

	s_Data.LineVertexCount = 0;
	s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

	s_Data.UIIndexCount = 0;
	s_Data.UIVertexBufferPtr = s_Data.UIVertexBufferBase;

	s_Data.TextRenderCommandCount = 0;

	s_Data.TextureSlotIndex = 1;
}

void Renderer2D_BeginScene(const Mat& viewProjection)
{
	s_Data.SceneBuffer.ViewProjection = DirectX::XMMatrixTranspose(viewProjection);

	ConstantBuffer_SetData(s_Data.CameraConstantBuffer, &s_Data.SceneBuffer);

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
			Texture2D_Bind(*s_Data.Textures[i], i);

		RendererAPI_DrawIndexed(s_Data.QuadVertexBuffer, s_Data.QuadIndexBuffer, s_Data.QuadPipeline, s_Data.QuadIndexCount);
	}

	if (s_Data.CircleIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
		VertexBuffer_SetData(s_Data.CircleVertexBuffer, s_Data.CircleVertexBufferBase, dataSize);

		// Use quad QuadIndexBuffer
		RendererAPI_DrawIndexed(s_Data.CircleVertexBuffer, s_Data.QuadIndexBuffer, s_Data.CirclePipeline, s_Data.CircleIndexCount);
	}

	if (s_Data.LineVertexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
		VertexBuffer_SetData(s_Data.LineVertexBuffer, s_Data.LineVertexBufferBase, dataSize);

		RendererAPI_DrawLines(s_Data.LineVertexBuffer, s_Data.LinePipeline, s_Data.LineVertexCount);
	}

	//RendererAPI_SetDepthTest(false);
	RendererAPI_SetBlendingState(BlendMode_Alpha);
	if (s_Data.UIIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.UIVertexBufferPtr - (uint8_t*)s_Data.UIVertexBufferBase);
		VertexBuffer_SetData(s_Data.UIVertexBuffer, s_Data.UIVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			Texture2D_Bind(*s_Data.Textures[i], i);

		RendererAPI_DrawIndexed(s_Data.UIVertexBuffer, s_Data.QuadIndexBuffer, s_Data.UIPipeline, s_Data.UIIndexCount);
	}
	//RendererAPI_SetDepthTest(true);
	RendererAPI_SetBlendingState(BlendMode_Disabled);

	// Rendering Text after all d3d11 draw calls, to make sure text is always on top (beacuse text is rendered by d2d)
	if (s_Data.TextRenderCommandCount)
	{
		for (size_t i = 0; i < s_Data.TextRenderCommandCount; i++)
		{
			TextRenderCommand* temp = (TextRenderCommand*)List_Get(s_Data.TextRenderCommands, i);
			RendererAPI_DrawText(
				temp->Text,
				temp->FontFamilyName,
				temp->Position,
				temp->Color,
				temp->FontSize
			);
		}
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

float GetTextureID(Texture2D& texture)
{
	float texIndex = 0.0f;
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
	{
		if (Texture2D_IsSame(*s_Data.Textures[i], texture))
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
		s_Data.Textures[s_Data.TextureSlotIndex] = &texture;
		s_Data.TextureSlotIndex++;
	}
	return texIndex;
}

void Renderer2D_DrawQuad(const Mat& transform, Texture2D& texture, const Vec2& uv0, const Vec2& uv1, const Vec4& tintColor, float tilingFactor)
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

void SetUIVertex(const Vec2& pos, const Vec2& size,
	const Vec4& color, const Vec2* texCoord, float texIndex, float tilingFactor)
{
	Vec2 Vertices[] = { { pos.x, pos.y }, { pos.x + size.x, pos.y }, { pos.x + size.x, pos.y + size.y }, { pos.x, pos.y + size.y } };

	for (size_t i = 0; i < 4; i++)
	{
		s_Data.UIVertexBufferPtr->Position = { Vertices[i].x, Vertices[i].y, 0.0f };
		s_Data.UIVertexBufferPtr->Color = color;
		s_Data.UIVertexBufferPtr->TexCoord = texCoord[i];
		s_Data.UIVertexBufferPtr->TexIndex = texIndex;
		s_Data.UIVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.UIVertexBufferPtr++;
	}
	s_Data.UIIndexCount += 6;
}

void Renderer2D_DrawUI(const Vec2& pos, const Vec2& size, const Vec4& color)
{
	const float texIndex = 0.0f; // White Texture
	const float tilingFactor = 1.0f;

	SetUIVertex(pos, size, color, s_Data.QuadTexCoord, texIndex, tilingFactor);
}

void Renderer2D_DrawUI(const Vec2& pos, const Vec2& size, Texture2D& texture, const Vec2& uv0, const Vec2& uv1, const Vec4& tintColor, float tilingFactor)
{
	Vec2 textureCoords[] = { uv0, { uv1.x, uv0.y }, uv1, { uv0.x, uv1.y } };
	SetUIVertex(pos, size, tintColor, textureCoords, GetTextureID(texture), tilingFactor);
}

void Renderer2D_DrawText(const WCHAR* str, const WCHAR* fontFamilyName, const Vec2& pos, const Vec4& color, float fontSize)
{
	TextRenderCommand* command = (TextRenderCommand*)List_Get(s_Data.TextRenderCommands, s_Data.TextRenderCommandCount);
	command->Text = str;
	command->FontFamilyName = fontFamilyName;
	command->Position = pos;
	command->Color = color;
	command->FontSize = fontSize;
	s_Data.TextRenderCommandCount++;

	CORE_ASSERT(s_Data.TextRenderCommandCount < COMMAND_BUFFER_SIZE, "Renderer2D_DrawText: command buffer overflow");
}