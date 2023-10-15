#pragma once
#include "Utils/HashMap.h"

#include <Windows.h>
#include <d3d11.h>

enum ShaderType
{
	ShaderType_Vertex = BIT(0),
	ShaderType_Pixel = BIT(1),
	ShaderType_Compute = BIT(2),

	ShaderType_All = ShaderType_Vertex | ShaderType_Pixel | ShaderType_Compute,
	ShaderType_Default = ShaderType_Vertex | ShaderType_Pixel,
};

struct Shader
{
	ID3D11VertexShader* VertexShader = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;
	ID3D11ComputeShader* ComputeShader = nullptr;

	ID3DBlob* VertexShaderBlob = nullptr;
	HashMap ReflectionData;
};

void Shader_Create(Shader& shader, const char* name, ShaderType type = ShaderType_Default);
const HashMap& Shader_GetReflectionData(const Shader& shader);
void Shader_Bind(const Shader& shader);
void Shader_Release(Shader& shader);