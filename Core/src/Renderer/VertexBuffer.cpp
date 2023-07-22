#include "pch.h"
#include "VertexBuffer.h"
#include "DXTrace.h"
#include "RendererContext.h"

static uint32_t ShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    return 4;
	case ShaderDataType::Float2:   return 4 * 2;
	case ShaderDataType::Float3:   return 4 * 3;
	case ShaderDataType::Float4:   return 4 * 4;
	case ShaderDataType::Int:      return 4;
	case ShaderDataType::Int2:     return 4 * 2;
	case ShaderDataType::Int3:     return 4 * 3;
	case ShaderDataType::Int4:     return 4 * 4;
	case ShaderDataType::Bool:     return 1;
	}
	BV_ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}

void VertexBufferLayout_CalculateOffsetsAndStride(VertexBufferLayout& out)
{
	uint32_t offset = 0;
	out.Stride = 0;
	for (uint32_t i = 0; i < out.ElementCount; i++)
	{
		out.Elements[i].Size = ShaderDataTypeSize(out.Elements[i].Type);
		out.Elements[i].Offset = offset;
		offset += out.Elements[i].Size;
		out.Stride += out.Elements[i].Size;
	}
}

void VertexBuffer_Create(VertexBuffer& out, uint32_t size)
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = out.Stride;
	BV_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateBuffer(&bufferDesc, nullptr, &out.Buffer));
}

void VertexBuffer_SetData(VertexBuffer& out, void* data, uint32_t size)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	BV_CHECK_DX_RESULT(RendererContext_GetDeviceContext()->Map(out.Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, data, size);
	RendererContext_GetDeviceContext()->Unmap(out.Buffer, 0);
}

void VertexBuffer_SetLayout(VertexBuffer& out, VertexBufferLayout& layout)
{
	out.Layout = layout;
	out.Stride = layout.Stride;
}

void VertexBuffer_Bind(const VertexBuffer& out)
{
	const UINT offset = 0;
	RendererContext_GetDeviceContext()->IASetVertexBuffers(0, 1, &out.Buffer, &out.Stride, &offset);
}

void VertexBuffer_Release(VertexBuffer& out)
{
	if (out.Buffer)
	{
		out.Buffer->Release();
		out.Buffer = nullptr;
	}
}