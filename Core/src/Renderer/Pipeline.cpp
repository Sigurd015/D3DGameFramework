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

void Pipeline_Create(Pipeline* out, const PipelineSpecification* spec)
{
	out->Spec = *spec;
	out->Layout = spec->Layout;
	out->Shader = spec->Shader;

	List temp;
	List_Create(&temp);
	for (size_t i = 0; i < out->Layout->ElementCount; i++)
	{
		D3D11_INPUT_ELEMENT_DESC* tempDesc = (D3D11_INPUT_ELEMENT_DESC*)malloc(sizeof(D3D11_INPUT_ELEMENT_DESC));
		*tempDesc = {
			out->Layout->Elements[i].Name,0,ShaderDataTypeToDX11BaseType(out->Layout->Elements[i].Type),
			0,(UINT)out->Layout->Elements[i].Offset ,D3D11_INPUT_PER_VERTEX_DATA ,0 };
		List_Add(&temp, tempDesc);
	}

	D3D11_INPUT_ELEMENT_DESC* tempList = (D3D11_INPUT_ELEMENT_DESC*)malloc(sizeof(D3D11_INPUT_ELEMENT_DESC) * List_Size(&temp));
	for (size_t i = 0; i < List_Size(&temp); i++)
	{
		tempList[i] = *(D3D11_INPUT_ELEMENT_DESC*)List_Get(&temp, i);
	}

	BV_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateInputLayout(
		tempList, (UINT)List_Size(&temp), out->Shader->VertexShaderBlob->GetBufferPointer(),
		out->Shader->VertexShaderBlob->GetBufferSize(), &out->InputLayout));

	free(tempList);
	List_Free(&temp);
}

void Pipeline_Bind(Pipeline* out)
{
	RendererContext_GetDeviceContext()->IASetInputLayout(out->InputLayout);
}

void Pipeline_Release(Pipeline* out)
{
	out->InputLayout->Release();
	Shader_Release(out->Shader);
}