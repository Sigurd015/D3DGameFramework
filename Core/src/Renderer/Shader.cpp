#include "pch.h"
#include "Shader.h"
#include "RendererContext.h"
#include "DXTrace.h"

#include <d3dcompiler.h>
#include <d3d11Shader.h>
#include <atlbase.h>
#include <atlconv.h>

#define SHADER_CACHE_DIR "assets/shaders/cache/"

void CreateReflectionData(Shader& shader, ID3DBlob* shaderBlob)
{
	ID3D11ShaderReflection* shaderReflection;
	D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&shaderReflection);

	D3D11_SHADER_DESC shaderDesc;
	shaderReflection->GetDesc(&shaderDesc);

	for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		shaderReflection->GetResourceBindingDesc(i, &bindDesc);

		if (bindDesc.Type == D3D_SIT_CBUFFER || bindDesc.Type == D3D_SIT_TEXTURE
			|| bindDesc.Type == D3D_SIT_SAMPLER || bindDesc.Type == D3D_SIT_TBUFFER)
		{
			HashMap_Set(shader.ReflectionData, bindDesc.Name, &bindDesc.BindPoint, sizeof(uint32_t));
		}
	}
}

void Shader_Create(Shader& shader, const char* name, ShaderType type)
{
	HashMap_Create(shader.ReflectionData);

	if (type & ShaderType_Vertex)
	{
		char vertexShaderName[256];
		sprintf_s(vertexShaderName, 256, "%s%s_v.cso", SHADER_CACHE_DIR, name);

		CORE_CHECK_DX_RESULT(D3DReadFileToBlob(CA2T(vertexShaderName), &shader.VertexShaderBlob));
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateVertexShader(shader.VertexShaderBlob->GetBufferPointer(),
			shader.VertexShaderBlob->GetBufferSize(), nullptr, &shader.VertexShader));

		CreateReflectionData(shader, shader.VertexShaderBlob);
	}

	if (type & ShaderType_Pixel)
	{
		char pixelShaderName[256];
		sprintf_s(pixelShaderName, 256, "%s%s_p.cso", SHADER_CACHE_DIR, name);

		ID3DBlob* blob;
		CORE_CHECK_DX_RESULT(D3DReadFileToBlob(CA2T(pixelShaderName), &blob));
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreatePixelShader(blob->GetBufferPointer(),
			blob->GetBufferSize(), nullptr, &shader.PixelShader));

		CreateReflectionData(shader, blob);
		blob->Release();
	}

	if (type & ShaderType_Compute)
	{
		char computeShaderName[256];
		sprintf_s(computeShaderName, 256, "%s%s_c.cso", SHADER_CACHE_DIR, name);

		ID3DBlob* blob;
		CORE_CHECK_DX_RESULT(D3DReadFileToBlob(CA2T(computeShaderName), &blob));
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateComputeShader(blob->GetBufferPointer(),
			blob->GetBufferSize(), nullptr, &shader.ComputeShader));

		CreateReflectionData(shader, blob);
		blob->Release();
	}
}

const HashMap& Shader_GetReflectionData(const Shader& shader)
{
	return shader.ReflectionData;
}

void Shader_Bind(const Shader& shader)
{
	RendererContext_GetDeviceContext()->VSSetShader(shader.VertexShader, nullptr, 0);
	RendererContext_GetDeviceContext()->PSSetShader(shader.PixelShader, nullptr, 0);
	RendererContext_GetDeviceContext()->CSSetShader(shader.ComputeShader, nullptr, 0);
}

void Shader_Release(Shader& shader)
{
	if (shader.VertexShaderBlob)
	{
		shader.VertexShaderBlob->Release();
		shader.VertexShaderBlob = nullptr;
	}
	if (shader.VertexShader)
	{
		shader.VertexShader->Release();
		shader.VertexShader = nullptr;
	}
	if (shader.PixelShader)
	{
		shader.PixelShader->Release();
		shader.PixelShader = nullptr;
	}
	if (shader.ComputeShader)
	{
		shader.ComputeShader->Release();
		shader.ComputeShader = nullptr;
	}

	HashMap_Free(shader.ReflectionData, true);
}