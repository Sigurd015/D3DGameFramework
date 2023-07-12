#pragma once
#include "Core/base.h"
#include "Math/Math.h"

#include <Windows.h>
#include <d3d11.h>

enum ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Bool
};

struct VertexBufferLayoutEmelent
{
	ShaderDataType Type;
	const char* Name;
	uint32_t Size = 0;
	size_t Offset = 0;
};

struct VertexBufferLayout
{
	VertexBufferLayoutEmelent* Elements;
	uint32_t ElementCount;
	uint32_t Stride = 0;
};

struct VertexBuffer
{
	ID3D11Buffer* Buffer;
	uint32_t Stride = 0;
	VertexBufferLayout Layout;
};

struct IndexBuffer
{
	ID3D11Buffer* Buffer;
	uint32_t Count;
};

struct Shader
{
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;

	ID3DBlob* VertexShaderBlob;
};

struct Pipeline
{
	Shader* Shader;
	VertexBufferLayout* Layout;
	ID3D11InputLayout* InputLayout;
};

struct ConstantBuffer
{
	ID3D11Buffer* Buffer;
	uint32_t Size;
	uint32_t BindSlot;
};

struct Texture2D
{
	ID3D11Texture2D* Texture;
	ID3D11ShaderResourceView* SRV;
	ID3D11SamplerState* SamplerState;
};

void RendererContext_Initialize(HWND* windowHandle);
void RendererContext_SwapBuffer(bool VSync);

void RendererAPI_Initialize();
void RendererAPI_SetViewport(uint32_t width, uint32_t height);
void RendererAPI_SetClearColor(const Vec4* color);
void RendererAPI_Clear();
void RendererAPI_DrawIndexed(const VertexBuffer* vertexBuffer, const IndexBuffer* indexBuffer, uint32_t indexCount);
void RendererAPI_DrawLines(const VertexBuffer* vertexBuffer, uint32_t vertexCount);

void VertexBufferLayout_CalculateOffsetsAndStride(VertexBufferLayout* out);

void VertexBuffer_Create(VertexBuffer* out, uint32_t size);
void VertexBuffer_SetData(VertexBuffer* out, void* data, uint32_t size);
void VertexBuffer_SetLayout(VertexBuffer* out, VertexBufferLayout* layout);
void VertexBuffer_Bind(VertexBuffer* out);

void IndexBuffer_Create(IndexBuffer* out, void* indices, uint32_t count);
void IndexBuffer_Bind(IndexBuffer* out);

void Shader_Create(Shader* out, const char* name);
void Shader_Bind(Shader* out);

void Pipeline_Create(Pipeline* out, Shader* shader, VertexBufferLayout* layout);
void Pipeline_Bind(Pipeline* out);

void ConstantBuffer_Create(ConstantBuffer* out, uint32_t size, uint32_t bindSlot);
void ConstantBuffer_SetData(ConstantBuffer* out, void* data);
void ConstantBuffer_Bind(ConstantBuffer* out);

void Texture2D_Create(Texture2D* out, const char* path);
void Texture2D_Bind(Texture2D* out, uint32_t slot);