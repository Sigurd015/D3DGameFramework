#include "pch.h"
#include "DXTrace.h"
#include "RendererContext.h"

#include "Pipeline.h"

static D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopologyTypeToD3D(PrimitiveTopology type)
{
	switch (type)
	{
	case PrimitiveTopology::PrimitiveTopology_Points:
		return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	case PrimitiveTopology::PrimitiveTopology_Lines:
		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case PrimitiveTopology::PrimitiveTopology_Triangles:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	CORE_ASSERT(false, "Unknown Primitive Topology!");
}

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
	CORE_ASSERT(false, "Unknown ShaderDataType!");
	return DXGI_FORMAT_UNKNOWN;
}

void Pipeline_Create(Pipeline& pipeline, const PipelineSpecification& spec)
{
	pipeline.Spec = spec;

	uint32_t memSize = sizeof(D3D11_INPUT_ELEMENT_DESC) * pipeline.Spec.Layout.ElementCount;
	D3D11_INPUT_ELEMENT_DESC* tempList = (D3D11_INPUT_ELEMENT_DESC*)Memory_Allocate(memSize, MemoryBlockTag_Array);
	for (size_t i = 0; i < pipeline.Spec.Layout.ElementCount; i++)
	{
		tempList[i] = {
			pipeline.Spec.Layout.Elements[i].Name,0,ShaderDataTypeToDX11BaseType(pipeline.Spec.Layout.Elements[i].Type),
			0,(UINT)pipeline.Spec.Layout.Elements[i].Offset ,D3D11_INPUT_PER_VERTEX_DATA ,0 };
	}

	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateInputLayout(
		tempList, (UINT)pipeline.Spec.Layout.ElementCount, pipeline.Spec.Shader->VertexShaderBlob->GetBufferPointer(),
		pipeline.Spec.Shader->VertexShaderBlob->GetBufferSize(), &pipeline.InputLayout));

	Memory_Free(tempList, memSize, MemoryBlockTag_Array);
}

void Pipeline_Bind(const Pipeline& pipeline)
{
	RendererContext_GetDeviceContext()->IASetInputLayout(pipeline.InputLayout);
	RendererContext_GetDeviceContext()->IASetPrimitiveTopology(PrimitiveTopologyTypeToD3D(pipeline.Spec.Topology));
	Shader_Bind(pipeline.Spec.Shader);
}

const PipelineSpecification& Pipeline_GetSpecification(const Pipeline& pipeline)
{
	return pipeline.Spec;
}

void Pipeline_Release(Pipeline& pipeline)
{
	if (pipeline.InputLayout)
	{
		pipeline.InputLayout->Release();
		pipeline.InputLayout = nullptr;
	}
	Framebuffer_Release(pipeline.Spec.TargetFramebuffer);
}