#pragma once
#include "Shader.h"

#include <stdint.h>
#include <Windows.h>
#include <d3d11.h>

struct ConstantBuffer
{
	ID3D11Buffer* Buffer = nullptr;
	uint32_t Size;
};

void ConstantBuffer_Create(ConstantBuffer& constantBuffer, uint32_t size);
void ConstantBuffer_SetData(ConstantBuffer& constantBuffer, void* data);
void ConstantBuffer_Bind(const ConstantBuffer* constantBuffer, const ShaderResourceDeclaration* decl);
void ConstantBuffer_Release(ConstantBuffer& constantBuffer);