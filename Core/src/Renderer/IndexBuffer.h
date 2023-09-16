#pragma once

#include <stdint.h>
#include <d3d11.h>

struct IndexBuffer
{
	ID3D11Buffer* Buffer;
	uint32_t Count;
};

void IndexBuffer_Create(IndexBuffer& indexBuffer, void* indices, uint32_t count);
void IndexBuffer_Bind(const IndexBuffer& indexBuffer);
void IndexBuffer_Release(IndexBuffer& indexBuffer);