#pragma once

#include <Windows.h>
#include <d3d11.h>

struct Shader
{
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;

	ID3DBlob* VertexShaderBlob;
};

void Shader_Create(Shader& out, const char* name);
void Shader_Bind(const Shader& out);
void Shader_Release(Shader& out);