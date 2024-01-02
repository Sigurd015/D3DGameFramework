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

	List tempList;
	List_Create(tempList, sizeof(D3D11_INPUT_ELEMENT_DESC), pipeline.Spec.Layout.ElementCount);
	for (size_t i = 0; i < pipeline.Spec.Layout.ElementCount; i++)
	{
		D3D11_INPUT_ELEMENT_DESC elementDesc = {};
		elementDesc.SemanticName = pipeline.Spec.Layout.Elements[i].Name;
		elementDesc.SemanticIndex = 0;
		elementDesc.Format = ShaderDataTypeToDX11BaseType(pipeline.Spec.Layout.Elements[i].Type);
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = pipeline.Spec.Layout.Elements[i].Offset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		List_Add(tempList, &elementDesc);
	}

	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateInputLayout(
		(D3D11_INPUT_ELEMENT_DESC*)List_GetData(tempList), List_Size(tempList),
		pipeline.Spec.Shader->VertexShaderBlob->GetBufferPointer(),
		pipeline.Spec.Shader->VertexShaderBlob->GetBufferSize(),
		&pipeline.InputLayout));

	List_Free(tempList);
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

void Pipeline_SetFramebuffer(Pipeline& pipeline, Framebuffer* framebuffer)
{
	pipeline.Spec.TargetFramebuffer = framebuffer;
}

void Pipeline_Release(Pipeline& pipeline)
{
	// Framebuffer release is handled by the RendererResourcePool
	if (pipeline.InputLayout)
	{
		pipeline.InputLayout->Release();
		pipeline.InputLayout = nullptr;
	}
}