#include "pch.h"
#include "DXTrace.h"
#include "RendererContext.h"

#include "Pipeline.h"

static DXGI_FORMAT ShaderDataTypeToDX11BaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
	case ShaderDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
	case ShaderDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
	case ShaderDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case ShaderDataType::Int:      return DXGI_FORMAT_R32_SINT;
	case ShaderDataType::Int2:     return DXGI_FORMAT_R32G32_SINT;
	case ShaderDataType::Int3:     return DXGI_FORMAT_R32G32B32_SINT;
	case ShaderDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_SINT;
	}
	BV_ASSERT(false, "Unknown ShaderDataType!");
	return DXGI_FORMAT_UNKNOWN;
}

void Pipeline_Create(Pipeline& out, const PipelineSpecification& spec)
{
	out.Spec = spec;
	out.Layout = *spec.Layout;
	out.Shader = *spec.Shader;

	D3D11_INPUT_ELEMENT_DESC* tempList = (D3D11_INPUT_ELEMENT_DESC*)malloc(sizeof(D3D11_INPUT_ELEMENT_DESC) * out.Layout.ElementCount);
	for (size_t i = 0; i < out.Layout.ElementCount; i++)
	{
		tempList[i] = {
			out.Layout.Elements[i].Name,0,ShaderDataTypeToDX11BaseType(out.Layout.Elements[i].Type),
			0,(UINT)out.Layout.Elements[i].Offset ,D3D11_INPUT_PER_VERTEX_DATA ,0 };
	}

	BV_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateInputLayout(
		tempList, (UINT)out.Layout.ElementCount, out.Shader.VertexShaderBlob->GetBufferPointer(),
		out.Shader.VertexShaderBlob->GetBufferSize(), &out.InputLayout));

	free(tempList);

	List_Create(out.ConstantBuffers);
}

void Pipeline_SetConstantBuffer(Pipeline& out, const ConstantBuffer& constantBuffer)
{
	List_Add(out.ConstantBuffers, (void*)&constantBuffer);
}

void Pipeline_Bind(const Pipeline& out)
{
	RendererContext_GetDeviceContext()->IASetInputLayout(out.InputLayout);

	Shader_Bind(out.Shader);

	uint32_t size = List_Size(out.ConstantBuffers);
	if (size)
	{
		for (size_t i = 0; i < size; i++)
		{
			ConstantBuffer_Bind(*(ConstantBuffer*)List_Get(out.ConstantBuffers, i));
		}
	}
}

void Pipeline_Release(Pipeline& out)
{
	out.InputLayout->Release();
	Shader_Release(out.Shader);
	List_Free(out.ConstantBuffers);
}