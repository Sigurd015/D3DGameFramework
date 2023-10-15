#pragma once
#include "Core/Base.h"

#include <Windows.h>
#include <d3d11.h>
#include <stdint.h>

enum ShaderDataType
{
	NoneType = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Bool
};

struct VertexBufferLayoutEmelent
{
	ShaderDataType Type;
	const char* Name;
	uint32_t Size = 0;
	uint32_t Offset = 0;
};

struct VertexBufferLayout
{
	VertexBufferLayoutEmelent* Elements;
	uint32_t ElementCount;
	uint32_t Stride = 0;
};

struct VertexBuffer
{
	ID3D11Buffer* Buffer = nullptr;
	uint32_t Stride = 0;
};

void VertexBufferLayout_CalculateOffsetsAndStride(VertexBufferLayout& layout);

void VertexBuffer_Create(VertexBuffer& vertexBuffer, uint32_t size);
void VertexBuffer_SetData(VertexBuffer& vertexBuffer, void* data, uint32_t size);
void VertexBuffer_SetLayout(VertexBuffer& vertexBuffer, VertexBufferLayout& layout);
void VertexBuffer_Bind(const VertexBuffer& vertexBuffer);
void VertexBuffer_Release(VertexBuffer& vertexBuffer);