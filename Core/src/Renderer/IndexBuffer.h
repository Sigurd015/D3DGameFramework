#pragma once

#include <stdint.h>
#include <d3d11.h>

struct IndexBuffer
{
	ID3D11Buffer* Buffer;
	uint32_t Count;
};

void IndexBuffer_Create(IndexBuffer* out, void* indices, uint32_t count);
void IndexBuffer_Bind(IndexBuffer* out);