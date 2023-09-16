#pragma once

#include <stdint.h>
#include <Windows.h>
#include <d3d11.h>

struct ConstantBuffer
{
	ID3D11Buffer* Buffer;
	uint32_t Size;
};

void ConstantBuffer_Create(ConstantBuffer& constantBuffer, uint32_t size);
void ConstantBuffer_SetData(ConstantBuffer& constantBuffer, void* data);
void ConstantBuffer_Bind(const ConstantBuffer* constantBuffer, uint32_t bindSlot);
void ConstantBuffer_Release(ConstantBuffer* constantBuffer);