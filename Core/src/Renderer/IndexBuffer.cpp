#include "pch.h"
#include "IndexBuffer.h"
#include "RendererContext.h"
#include "DXTrace.h"

void IndexBuffer_Create(IndexBuffer& indexBuffer, void* indices, uint32_t size)
{
	indexBuffer.Count = size / sizeof(uint32_t);

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = indices;

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateBuffer(&bufferDesc, &resourceData, &indexBuffer.Buffer));
}

void IndexBuffer_Bind(const IndexBuffer& indexBuffer)
{
	RendererContext_GetDeviceContext()->IASetIndexBuffer(indexBuffer.Buffer, DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer_Release(IndexBuffer& indexBuffer)
{
	if (indexBuffer.Buffer)
	{
		indexBuffer.Buffer->Release();
		indexBuffer.Buffer = nullptr;
	}
}

uint32_t IndexBuffer_GetCount(const IndexBuffer& indexBuffer)
{
	return indexBuffer.Count;
}