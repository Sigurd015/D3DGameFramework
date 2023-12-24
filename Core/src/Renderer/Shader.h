#pragma once
#include "Utils/List.h"

#include <Windows.h>
#include <d3d11.h>

enum ShaderType
{
	ShaderType_Vertex = BIT(0),
	ShaderType_Geometry = BIT(1),
	ShaderType_Pixel = BIT(2),
	ShaderType_Compute = BIT(3),

	ShaderType_Geo = ShaderType_Vertex | ShaderType_Geometry | ShaderType_Pixel,
	ShaderType_Default = ShaderType_Vertex | ShaderType_Pixel,
};

enum ShaderResourceType
{
	ShaderResourceType_Resource,
	ShaderResourceType_Sampler,
	ShaderResourceType_UnorderedAccess,
	ShaderResourceType_ConstantBuffer,
	ShaderResourceType_StructuredBuffer,
};

struct ShaderResourceDeclaration
{
	char* Name;
	ShaderResourceType ResourceType;
	uint32_t Slot;
	ShaderType Stage;
};

struct Shader
{
	ID3D11VertexShader* VertexShader = nullptr;
	ID3D11GeometryShader* GeometryShader = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;
	ID3D11ComputeShader* ComputeShader = nullptr;

	ID3DBlob* VertexShaderBlob = nullptr;
	List ReflectionData;
};

void Shader_Create(Shader& shader, const char* name, ShaderType type = ShaderType_Default);
const List& Shader_GetReflectionData(const Shader& shader);
void Shader_Bind(const Shader& shader);
void Shader_Release(Shader& shader);