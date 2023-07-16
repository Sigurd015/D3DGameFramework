#include "pch.h"
#include "ConstantBuffer.h"
#include "RendererContext.h"
#include "DXTrace.h"

void ConstantBuffer_Create(ConstantBuffer* out, uint32_t size, uint32_t bindSlot)
{
	out->Size = size;
	out->BindSlot = bindSlot;

	D3D11_BUFFER_DESC buffer = {};
	buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer.Usage = D3D11_USAGE_DYNAMIC;
	buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer.MiscFlags = 0;
	buffer.ByteWidth = size;
	buffer.StructureByteStride = 0;
	BV_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateBuffer(&buffer, nullptr, &out->Buffer));
}

void ConstantBuffer_SetData(ConstantBuffer* out, void* data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	BV_CHECK_DX_RESULT(RendererContext_GetDeviceContext()->Map(out->Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, data, out->Size);
	RendererContext_GetDeviceContext()->Unmap(out->Buffer, 0);
}

void ConstantBuffer_Bind(ConstantBuffer* out)
{
	RendererContext_GetDeviceContext()->VSSetConstantBuffers(out->BindSlot, 1, &out->Buffer);
	RendererContext_GetDeviceContext()->PSSetConstantBuffers(out->BindSlot, 1, &out->Buffer);
}

void ConstantBuffer_Release(ConstantBuffer* out)
{
	out->Buffer->Release();
}