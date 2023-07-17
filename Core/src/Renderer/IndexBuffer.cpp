#include "pch.h"
#include "IndexBuffer.h"
#include "RendererContext.h"
#include "DXTrace.h"

void IndexBuffer_Create(IndexBuffer& out, void* indices, uint32_t count)
{
	out.Count = count;

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = indices;

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = count * sizeof(uint32_t);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	BV_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateBuffer(&bufferDesc, &resourceData, &out.Buffer));
}

void IndexBuffer_Bind(const IndexBuffer& out)
{
	RendererContext_GetDeviceContext()->IASetIndexBuffer(out.Buffer, DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer_Release(IndexBuffer& out)
{
	out.Buffer->Release();
}