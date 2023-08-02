#include "pch.h"
#include "Shader.h"
#include "RendererContext.h"
#include "DXTrace.h"

#include <d3dcompiler.h>
#include <atlbase.h>
#include <atlconv.h>

void Shader_Create(Shader& out, const char* name)
{
	char vertexShaderName[256];
	sprintf_s(vertexShaderName, 256, "assets/shaders/cache/%s_v.cso", name);
	char pixelShaderName[256];
	sprintf_s(pixelShaderName, 256, "assets/shaders/cache/%s_p.cso", name);

	CORE_CHECK_DX_RESULT(D3DReadFileToBlob(CA2T(vertexShaderName), &out.VertexShaderBlob));
	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateVertexShader(out.VertexShaderBlob->GetBufferPointer(),
		out.VertexShaderBlob->GetBufferSize(), nullptr, &out.VertexShader));

	ID3DBlob* blob;
	CORE_CHECK_DX_RESULT(D3DReadFileToBlob(CA2T(pixelShaderName), &blob));
	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreatePixelShader(blob->GetBufferPointer(),
		blob->GetBufferSize(), nullptr, &out.PixelShader));

	blob->Release();
}

void Shader_Bind(const Shader& out)
{
	RendererContext_GetDeviceContext()->VSSetShader(out.VertexShader, nullptr, 0);
	RendererContext_GetDeviceContext()->PSSetShader(out.PixelShader, nullptr, 0);
}

void Shader_Release(Shader& out)
{
	if (out.VertexShaderBlob)
	{
		out.VertexShaderBlob->Release();
		out.VertexShaderBlob = nullptr;
	}
	if (out.VertexShader)
	{
		out.VertexShader->Release();
		out.VertexShader = nullptr;
	}
	if (out.PixelShader)
	{
		out.PixelShader->Release();
		out.PixelShader = nullptr;
	}
}