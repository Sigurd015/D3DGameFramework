#pragma once
#include "Core/base.h"

#include <Windows.h>
#include <d3d11.h>
#include <stdint.h>

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

void VertexBufferLayout_CalculateOffsetsAndStride(VertexBufferLayout* out);

void VertexBuffer_Create(VertexBuffer* out, uint32_t size);
void VertexBuffer_SetData(VertexBuffer* out, void* data, uint32_t size);
void VertexBuffer_SetLayout(VertexBuffer* out, VertexBufferLayout* layout);
void VertexBuffer_Bind(VertexBuffer* out);