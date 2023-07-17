#pragma once

#include <stdint.h>
#include <Windows.h>
#include <d3d11.h>

struct ConstantBuffer
{
	ID3D11Buffer* Buffer;
	uint32_t Size;
	uint32_t BindSlot;
};

void ConstantBuffer_Create(ConstantBuffer& out, uint32_t size, uint32_t bindSlot);
void ConstantBuffer_SetData(ConstantBuffer& out, void* data);
void ConstantBuffer_Bind(const ConstantBuffer& out);
void ConstantBuffer_Release(ConstantBuffer& out);