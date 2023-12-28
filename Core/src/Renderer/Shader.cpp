#include "pch.h"
#include "Shader.h"
#include "RendererContext.h"
#include "DXTrace.h"

#include <d3dcompiler.h>
#include <d3d11Shader.h>

#define SHADER_CACHE_DIR "assets/shaders/cache/"

void CreateReflectionData(Shader& shader, ID3DBlob* shaderBlob, ShaderType shaderType)
{
	ID3D11ShaderReflection* shaderReflection;
	D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&shaderReflection);

	D3D11_SHADER_DESC shaderDesc;
	shaderReflection->GetDesc(&shaderDesc);

	for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		shaderReflection->GetResourceBindingDesc(i, &bindDesc);

		ShaderResourceDeclaration resourceDeclaration = {};
		if (bindDesc.Type == D3D_SIT_SAMPLER)
		{
			resourceDeclaration.Name = strdup(bindDesc.Name);
			resourceDeclaration.ResourceType = ShaderResourceType_Sampler;
			resourceDeclaration.Slot = bindDesc.BindPoint;
			resourceDeclaration.Stage = shaderType;
		}
		else if (bindDesc.Type == D3D_SIT_TEXTURE)
		{
			resourceDeclaration.Name = strdup(bindDesc.Name);
			resourceDeclaration.ResourceType = ShaderResourceType_Resource;
			resourceDeclaration.Slot = bindDesc.BindPoint;
			resourceDeclaration.Stage = shaderType;
		}
		else if (bindDesc.Type == D3D_SIT_UAV_RWTYPED)
		{
			resourceDeclaration.Name = strdup(bindDesc.Name);
			resourceDeclaration.ResourceType = ShaderResourceType_UnorderedAccess;
			resourceDeclaration.Slot = bindDesc.BindPoint;
			resourceDeclaration.Stage = shaderType;
		}
		else if (bindDesc.Type == D3D_SIT_CBUFFER)
		{
			resourceDeclaration.Name = strdup(bindDesc.Name);
			resourceDeclaration.ResourceType = ShaderResourceType_ConstantBuffer;
			resourceDeclaration.Slot = bindDesc.BindPoint;
			resourceDeclaration.Stage = shaderType;
		}
		else if (bindDesc.Type == D3D_SIT_STRUCTURED)
		{
			resourceDeclaration.Name = strdup(bindDesc.Name);
			resourceDeclaration.ResourceType = ShaderResourceType_StructuredBuffer;
			resourceDeclaration.Slot = bindDesc.BindPoint;
			resourceDeclaration.Stage = shaderType;
		}
		List_Add(shader.ReflectionData, &resourceDeclaration);
	}
}

void Shader_Create(Shader& shader, const char* name, ShaderType type)
{
	List_Create(shader.ReflectionData, sizeof(ShaderResourceDeclaration));

	if (type & ShaderType_Vertex)
	{
		char vertexShaderName[256];
		sprintf_s(vertexShaderName, 256, "%s%s_v.cso", SHADER_CACHE_DIR, name);

		size_t newsize = strlen(vertexShaderName) + 1;

		wchar_t* wcstring = new wchar_t[newsize];

		// Convert char* string to a wchar_t* string.
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, vertexShaderName, _TRUNCATE);

		CORE_CHECK_DX_RESULT(D3DReadFileToBlob(wcstring, &shader.VertexShaderBlob));
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateVertexShader(shader.VertexShaderBlob->GetBufferPointer(),
			shader.VertexShaderBlob->GetBufferSize(), nullptr, &shader.VertexShader));

		delete[]wcstring;

		CreateReflectionData(shader, shader.VertexShaderBlob, ShaderType_Vertex);
	}

	if (type & ShaderType_Geometry)
	{
		char geometryShaderName[256];
		sprintf_s(geometryShaderName, 256, "%s%s_g.cso", SHADER_CACHE_DIR, name);

		size_t newsize = strlen(geometryShaderName) + 1;

		wchar_t* wcstring = new wchar_t[newsize];

		// Convert char* string to a wchar_t* string.
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, geometryShaderName, _TRUNCATE);

		ID3DBlob* blob;
		CORE_CHECK_DX_RESULT(D3DReadFileToBlob(wcstring, &blob));
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateGeometryShader(blob->GetBufferPointer(),
			blob->GetBufferSize(), nullptr, &shader.GeometryShader));

		delete[]wcstring;

		CreateReflectionData(shader, blob, ShaderType_Geometry);
		blob->Release();
	}

	if (type & ShaderType_Pixel)
	{
		char pixelShaderName[256];
		sprintf_s(pixelShaderName, 256, "%s%s_p.cso", SHADER_CACHE_DIR, name);

		size_t newsize = strlen(pixelShaderName) + 1;

		wchar_t* wcstring = new wchar_t[newsize];

		// Convert char* string to a wchar_t* string.
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, pixelShaderName, _TRUNCATE);

		ID3DBlob* blob;
		CORE_CHECK_DX_RESULT(D3DReadFileToBlob(wcstring, &blob));
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreatePixelShader(blob->GetBufferPointer(),
			blob->GetBufferSize(), nullptr, &shader.PixelShader));

		delete[]wcstring;

		CreateReflectionData(shader, blob, ShaderType_Pixel);
		blob->Release();
	}

	if (type & ShaderType_Compute)
	{
		char computeShaderName[256];
		sprintf_s(computeShaderName, 256, "%s%s_c.cso", SHADER_CACHE_DIR, name);

		size_t newsize = strlen(computeShaderName) + 1;

		wchar_t* wcstring = new wchar_t[newsize];

		// Convert char* string to a wchar_t* string.
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, computeShaderName, _TRUNCATE);

		ID3DBlob* blob;
		CORE_CHECK_DX_RESULT(D3DReadFileToBlob(wcstring, &blob));
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateComputeShader(blob->GetBufferPointer(),
			blob->GetBufferSize(), nullptr, &shader.ComputeShader));

		delete[]wcstring;

		CreateReflectionData(shader, blob, ShaderType_Compute);
		blob->Release();
	}
}

const List& Shader_GetReflectionData(const Shader& shader)
{
	return shader.ReflectionData;
}

void Shader_Bind(const Shader& shader)
{
	RendererContext_GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	RendererContext_GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	RendererContext_GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);
	RendererContext_GetDeviceContext()->CSSetShader(nullptr, nullptr, 0);

	RendererContext_GetDeviceContext()->VSSetShader(shader.VertexShader, nullptr, 0);
	RendererContext_GetDeviceContext()->GSSetShader(shader.GeometryShader, nullptr, 0);
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
	if (shader.GeometryShader)
	{
		shader.GeometryShader->Release();
		shader.GeometryShader = nullptr;
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

	List_Free(shader.ReflectionData);
}