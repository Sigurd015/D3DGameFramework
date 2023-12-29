#include "pch.h"
#include "Renderer2D.h"
#include "RendererAPI.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Pipeline.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Material.h"
#include "Asset/AssetManager.h"

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
	static const uint32_t MaxTextureSlots = 16;

	RenderPass QuadRenderPass;
	VertexBuffer QuadVertexBuffer;
	IndexBuffer QuadIndexBuffer;
	uint32_t QuadIndexCount = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;

	RenderPass CircleRenderPass;
	VertexBuffer CircleVertexBuffer;
	uint32_t CircleIndexCount = 0;
	CircleVertex* CircleVertexBufferBase = nullptr;
	CircleVertex* CircleVertexBufferPtr = nullptr;

	RenderPass LineRenderPass;
	VertexBuffer LineVertexBuffer;
	uint32_t LineVertexCount = 0;
	LineVertex* LineVertexBufferBase = nullptr;
	LineVertex* LineVertexBufferPtr = nullptr;
	float LineWidth = 2.0f;

	RenderPass UIRenderPass;
	VertexBuffer UIVertexBuffer;
	uint32_t UIIndexCount = 0;
	QuadVertex* UIVertexBufferBase = nullptr;
	QuadVertex* UIVertexBufferPtr = nullptr;

	List TextRenderCommands;

	Material DefaultMaterial;
	Texture2D* Textures[MaxTextureSlots];// 0 is white texture
	char* TextureSlotsNames[MaxTextureSlots];
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

	Renderer2DStatistics Stats;
};
static Renderer2DData s_Data;

void Renderer2D_Init()
{
	s_Data = {};
	//Quad
	{
		PipelineSpecification pipelineSpec;

		VertexBufferLayoutEmelent layoutEmelent[5] = {
		   { ShaderDataType::Float3, "a_Position"     },
		   { ShaderDataType::Float4, "a_Color"        },
		   { ShaderDataType::Float2, "a_TexCoord"     },
		   { ShaderDataType::Int,    "a_TexIndex"     },
		   { ShaderDataType::Float,  "a_TilingFactor" },
		};

		// Notice: The layoutEmelent not copyed, it's just a pointer.
		// When the pipeline created, the layoutEmelent should never be used again.
		pipelineSpec.Layout = { layoutEmelent, 5 };
		VertexBufferLayout_CalculateOffsetsAndStride(pipelineSpec.Layout);
		VertexBuffer_Create(s_Data.QuadVertexBuffer, s_Data.MaxVertices * sizeof(QuadVertex));
		VertexBuffer_SetLayout(s_Data.QuadVertexBuffer, pipelineSpec.Layout);

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
		IndexBuffer_Create(s_Data.QuadIndexBuffer, indices, s_Data.MaxIndices);
		delete[] indices;

		pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("Renderer2D_Quad", true);
		pipelineSpec.DepthTest = true;
		pipelineSpec.BackfaceCulling = true;
		pipelineSpec.DepthOperator = DepthCompareOperator_Less;
		pipelineSpec.Topology = PrimitiveTopology_Triangles;
		pipelineSpec.Blend = BlendMode_Disabled;

		RenderPassSpecification renderPassSpec;
		Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
		RenderPass_Create(s_Data.QuadRenderPass, renderPassSpec);
		Material_Create(s_Data.DefaultMaterial, pipelineSpec.Shader);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
	}

	// Circles
	{
		PipelineSpecification pipelineSpec;

		VertexBufferLayoutEmelent layoutEmelent[5] = {
		  { ShaderDataType::Float3, "a_WorldPosition" },
		  { ShaderDataType::Float3, "a_LocalPosition" },
		  { ShaderDataType::Float4, "a_Color"         },
		  { ShaderDataType::Float,  "a_Thickness"     },
		  { ShaderDataType::Float,  "a_Fade"          },
		};
		pipelineSpec.Layout = { layoutEmelent, 5 };
		VertexBufferLayout_CalculateOffsetsAndStride(pipelineSpec.Layout);
		VertexBuffer_Create(s_Data.CircleVertexBuffer, s_Data.MaxVertices * sizeof(CircleVertex));
		VertexBuffer_SetLayout(s_Data.CircleVertexBuffer, pipelineSpec.Layout);

		pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("Renderer2D_Circle", true);
		pipelineSpec.DepthTest = true;
		pipelineSpec.BackfaceCulling = true;
		pipelineSpec.DepthOperator = DepthCompareOperator_Less;
		pipelineSpec.Topology = PrimitiveTopology_Triangles;
		pipelineSpec.Blend = BlendMode_Disabled;

		RenderPassSpecification renderPassSpec;
		Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
		RenderPass_Create(s_Data.CircleRenderPass, renderPassSpec);

		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];
	}

	// Lines
	{
		PipelineSpecification pipelineSpec;

		VertexBufferLayoutEmelent layoutEmelent[2] = {
		  { ShaderDataType::Float3, "a_Position" },
		  { ShaderDataType::Float4, "a_Color"    },
		};
		pipelineSpec.Layout = { layoutEmelent, 2 };
		VertexBufferLayout_CalculateOffsetsAndStride(pipelineSpec.Layout);
		VertexBuffer_Create(s_Data.LineVertexBuffer, s_Data.MaxVertices * sizeof(LineVertex));
		VertexBuffer_SetLayout(s_Data.LineVertexBuffer, pipelineSpec.Layout);

		pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("Renderer2D_Line", true);
		pipelineSpec.DepthTest = true;
		pipelineSpec.BackfaceCulling = true;
		pipelineSpec.DepthOperator = DepthCompareOperator_Less;
		pipelineSpec.Topology = PrimitiveTopology_Lines;
		pipelineSpec.Blend = BlendMode_Disabled;

		RenderPassSpecification renderPassSpec;
		Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
		RenderPass_Create(s_Data.LineRenderPass, renderPassSpec);

		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];
	}

	// UI
	{
		PipelineSpecification pipelineSpec;

		VertexBufferLayoutEmelent layoutEmelent[5] = {
		   { ShaderDataType::Float3, "a_Position"     },
		   { ShaderDataType::Float4, "a_Color"        },
		   { ShaderDataType::Float2, "a_TexCoord"     },
		   { ShaderDataType::Int,    "a_TexIndex"     },
		   { ShaderDataType::Float,  "a_TilingFactor" },
		};
		pipelineSpec.Layout = { layoutEmelent, 5 };
		VertexBufferLayout_CalculateOffsetsAndStride(pipelineSpec.Layout);
		VertexBuffer_Create(s_Data.UIVertexBuffer, s_Data.MaxVertices * sizeof(TextVertex));
		VertexBuffer_SetLayout(s_Data.UIVertexBuffer, pipelineSpec.Layout);

		// Using the same shader as quads, but binding a identity viewProjection matrix.
		// Because UI position is already in screen space.
		pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("Renderer2D_Quad", true);
		pipelineSpec.DepthTest = false;
		pipelineSpec.BackfaceCulling = true;
		pipelineSpec.DepthOperator = DepthCompareOperator_Less;
		pipelineSpec.Topology = PrimitiveTopology_Triangles;
		pipelineSpec.Blend = BlendMode_Alpha;

		RenderPassSpecification renderPassSpec;
		Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
		RenderPass_Create(s_Data.UIRenderPass, renderPassSpec);

		s_Data.UIVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
	}

	// Set WhiteTexture slots to 0
	uint32_t whiteTextureData = 0xffffffff;
	TextureSpecification spec;
	spec.Width = 1;
	spec.Height = 1;
	spec.Format = ImageFormat::RGBA8;
	spec.Data = &whiteTextureData;
	spec.DataSize = sizeof(uint32_t);

	Texture2D_Create(&s_Data.WhiteTexture, spec);
	s_Data.Textures[0] = &s_Data.WhiteTexture;

	for (size_t i = 0; i < s_Data.MaxTextureSlots; i++)
	{
		//String_Format(s_Data.TextureSlotsNames[i], "u_Textures[%d]", i);
		s_Data.TextureSlotsNames[i] = new char[256];
		sprintf_s(s_Data.TextureSlotsNames[i], 256, "u_Textures[%d]", i);
	}

	ConstantBuffer_Create(s_Data.CameraConstantBuffer, sizeof(Renderer2DData::CameraData));
	RenderPass_SetInput(s_Data.QuadRenderPass, "Camera", RendererResourceType_ConstantBuffer, &s_Data.CameraConstantBuffer);
	RenderPass_SetInput(s_Data.CircleRenderPass, "Camera", RendererResourceType_ConstantBuffer, &s_Data.CameraConstantBuffer);
	RenderPass_SetInput(s_Data.LineRenderPass, "Camera", RendererResourceType_ConstantBuffer, &s_Data.CameraConstantBuffer);

	ConstantBuffer_Create(s_Data.IdentityConstantBuffer, sizeof(Renderer2DData::CameraData));
	static Mat identity = DirectX::XMMatrixIdentity();
	ConstantBuffer_SetData(s_Data.IdentityConstantBuffer, &identity);
	RenderPass_SetInput(s_Data.UIRenderPass, "Camera", RendererResourceType_ConstantBuffer, &s_Data.IdentityConstantBuffer);

	List_Create(s_Data.TextRenderCommands, sizeof(TextRenderCommand));
}

void Renderer2D_Shutdown()
{
	VertexBuffer_Release(s_Data.QuadVertexBuffer);
	VertexBuffer_Release(s_Data.CircleVertexBuffer);
	VertexBuffer_Release(s_Data.LineVertexBuffer);
	VertexBuffer_Release(s_Data.UIVertexBuffer);

	IndexBuffer_Release(s_Data.QuadIndexBuffer);

	RenderPass_Release(s_Data.QuadRenderPass);
	RenderPass_Release(s_Data.CircleRenderPass);
	RenderPass_Release(s_Data.LineRenderPass);
	RenderPass_Release(s_Data.UIRenderPass);

	ConstantBuffer_Release(s_Data.CameraConstantBuffer);
	ConstantBuffer_Release(s_Data.IdentityConstantBuffer);

	List_Free(s_Data.TextRenderCommands);

	delete[] s_Data.QuadVertexBufferBase;
	delete[] s_Data.CircleVertexBufferBase;
	delete[] s_Data.LineVertexBufferBase;
	delete[] s_Data.UIVertexBufferBase;

	for (size_t i = 0; i < s_Data.MaxTextureSlots; i++)
	{
		delete[] s_Data.TextureSlotsNames[i];
	}

	Texture2D_Release(&s_Data.WhiteTexture);
}

void StartBatch()
{
	Renderer2D_ResetStats();

	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	s_Data.CircleIndexCount = 0;
	s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

	s_Data.LineVertexCount = 0;
	s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

	s_Data.UIIndexCount = 0;
	s_Data.UIVertexBufferPtr = s_Data.UIVertexBufferBase;

	List_Clear(s_Data.TextRenderCommands);

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
	// Bind textures
	Material_Clear(s_Data.DefaultMaterial);
	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
	{
		Material_SetTexture(s_Data.DefaultMaterial, s_Data.TextureSlotsNames[i], s_Data.Textures[i]);
	}

	RendererAPI_BeginRenderPass(s_Data.QuadRenderPass, false);
	if (s_Data.QuadIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		VertexBuffer_SetData(s_Data.QuadVertexBuffer, s_Data.QuadVertexBufferBase, dataSize);

		RendererAPI_DrawIndexed(s_Data.QuadVertexBuffer, s_Data.QuadIndexBuffer, s_Data.DefaultMaterial, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}
	RendererAPI_EndRenderPass();

	RendererAPI_BeginRenderPass(s_Data.CircleRenderPass, false);
	if (s_Data.CircleIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
		VertexBuffer_SetData(s_Data.CircleVertexBuffer, s_Data.CircleVertexBufferBase, dataSize);

		// Use quad QuadIndexBuffer
		RendererAPI_DrawIndexed(s_Data.CircleVertexBuffer, s_Data.QuadIndexBuffer, s_Data.CircleIndexCount);
		s_Data.Stats.DrawCalls++;
	}
	RendererAPI_EndRenderPass();

	RendererAPI_BeginRenderPass(s_Data.LineRenderPass, false);
	if (s_Data.LineVertexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
		VertexBuffer_SetData(s_Data.LineVertexBuffer, s_Data.LineVertexBufferBase, dataSize);

		RendererAPI_DrawLines(s_Data.LineVertexBuffer, s_Data.LineVertexCount);
		s_Data.Stats.DrawCalls++;
	}
	RendererAPI_EndRenderPass();

	RendererAPI_BeginRenderPass(s_Data.UIRenderPass, false);
	if (s_Data.UIIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.UIVertexBufferPtr - (uint8_t*)s_Data.UIVertexBufferBase);
		VertexBuffer_SetData(s_Data.UIVertexBuffer, s_Data.UIVertexBufferBase, dataSize);

		RendererAPI_DrawIndexed(s_Data.UIVertexBuffer, s_Data.QuadIndexBuffer, s_Data.DefaultMaterial, s_Data.UIIndexCount);
		s_Data.Stats.DrawCalls++;
	}
	RendererAPI_EndRenderPass();

	// Rendering Text after all d3d11 draw calls, to make sure text is always on top (beacuse text is rendered by d2d)
	uint32_t commandCount = List_Size(s_Data.TextRenderCommands);
	if (commandCount)
	{
		for (size_t i = 0; i < commandCount; i++)
		{
			TextRenderCommand* temp = (TextRenderCommand*)List_Get(s_Data.TextRenderCommands, i);
			RendererAPI_DrawText(
				temp->Text,
				temp->FontFamilyName,
				temp->Position,
				temp->Color,
				temp->FontSize
			);
			s_Data.Stats.DrawCalls++;
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
	const Vec4& color, const Vec2* texCoord, uint32_t texIndex, float tilingFactor)
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

	s_Data.Stats.QuadCount++;
}

void Renderer2D_DrawQuad(const Mat& transform, const Vec4& color)
{
	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		NextBatch();

	const float texIndex = 0.0f; // White Texture
	const float tilingFactor = 1.0f;

	SetQuadVertex(transform, color, s_Data.QuadTexCoord, texIndex, tilingFactor);
}

uint32_t GetTextureID(Texture2D* texture)
{
	uint32_t texIndex = 0;
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
	{
		if (Texture2D_IsSame(s_Data.Textures[i], texture))
		{
			texIndex = i;
			break;
		}
	}

	if (texIndex == 0)
	{
		if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			NextBatch();

		texIndex = s_Data.TextureSlotIndex;
		s_Data.Textures[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;
	}
	return texIndex;
}

void Renderer2D_DrawQuad(const Mat& transform, Texture2D* texture, const Vec2& uv0, const Vec2& uv1, const Vec4& tintColor, float tilingFactor)
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

	s_Data.Stats.QuadCount++;
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

	s_Data.Stats.LineCount += 4;
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

	s_Data.Stats.LineCount += 4;
}

void SetUIVertex(const Vec2& pos, const Vec2& size, float rotation,
	const Vec4& color, const Vec2* texCoord, uint32_t texIndex, float tilingFactor)
{
	Vec2 Vertices[] = { { pos.x, pos.y }, { pos.x + size.x, pos.y }, { pos.x + size.x, pos.y + size.y }, { pos.x, pos.y + size.y } };
	Vec2 center = { pos.x + size.x * 0.5f, pos.y + size.y * 0.5f };
	for (size_t i = 0; i < 4; i++)
	{
		Vec2 tempPos = Vec2RotateByPivot(Vertices[i], center, rotation);
		s_Data.UIVertexBufferPtr->Position = { tempPos.x, tempPos.y, 0.0f };

		s_Data.UIVertexBufferPtr->Color = color;
		s_Data.UIVertexBufferPtr->TexCoord = texCoord[i];
		s_Data.UIVertexBufferPtr->TexIndex = texIndex;
		s_Data.UIVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.UIVertexBufferPtr++;
	}
	s_Data.UIIndexCount += 6;

	s_Data.Stats.QuadCount++;
}

void Renderer2D_DrawUI(const Vec2& pos, const Vec2& size, float rotation, const Vec4& color)
{
	const uint32_t texIndex = 0; // White Texture
	const float tilingFactor = 1.0f;

	SetUIVertex(pos, size, rotation, color, s_Data.QuadTexCoord, texIndex, tilingFactor);
}

void Renderer2D_DrawUI(const Vec2& pos, const Vec2& size, float rotation, Texture2D* texture, const Vec2& uv0, const Vec2& uv1, const Vec4& tintColor, float tilingFactor)
{
	Vec2 textureCoords[] = { uv0, { uv1.x, uv0.y }, uv1, { uv0.x, uv1.y } };
	SetUIVertex(pos, size, rotation, tintColor, textureCoords, GetTextureID(texture), tilingFactor);
}

void Renderer2D_DrawText(const WCHAR* str, const WCHAR* fontFamilyName, const Vec2& pos, const Vec4& color, float fontSize)
{
	TextRenderCommand command = {};
	command.Text = str;
	command.FontFamilyName = fontFamilyName;
	command.Position = pos;
	command.Color = color;
	command.FontSize = fontSize;
	List_Add(s_Data.TextRenderCommands, &command);
}

void Renderer2D_ResetStats()
{
	s_Data.Stats = {};
}

Renderer2DStatistics Renderer2D_GetStats()
{
	return s_Data.Stats;
}