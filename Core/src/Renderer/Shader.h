#pragma once
#include "Utils/HashMap.h"

#include <Windows.h>
#include <d3d11.h>

struct Shader
{
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;

	ID3DBlob* VertexShaderBlob;
	HashMap ReflectionData;
};

void Shader_Create(Shader& shader, const char* name);
const HashMap& Shader_GetReflectionData(const Shader& shader);
void Shader_Bind(const Shader& shader);
void Shader_Release(Shader& shader);