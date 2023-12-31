#include "pch.h"
#include "ConstantBuffer.h"
#include "RendererContext.h"
#include "DXTrace.h"

void ConstantBuffer_Create(ConstantBuffer& constantBuffer, uint32_t size)
{
	constantBuffer.Size = size;

	D3D11_BUFFER_DESC buffer = {};
	buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer.Usage = D3D11_USAGE_DYNAMIC;
	buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer.MiscFlags = 0;
	buffer.ByteWidth = size;
	buffer.StructureByteStride = 0;
	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateBuffer(&buffer, nullptr, &constantBuffer.Buffer));
}

void ConstantBuffer_SetData(ConstantBuffer& constantBuffer, void* data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	CORE_CHECK_DX_RESULT(RendererContext_GetDeviceContext()->Map(constantBuffer.Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, data, constantBuffer.Size);
	RendererContext_GetDeviceContext()->Unmap(constantBuffer.Buffer, 0);
}

void ConstantBuffer_Bind(const ConstantBuffer* constantBuffer, const ShaderResourceDeclaration* decl)
{
	switch (decl->Stage)
	{
	case ShaderType_Vertex: RendererContext_GetDeviceContext()->VSSetConstantBuffers(decl->Slot, 1, &constantBuffer->Buffer); break;
	case ShaderType_Geometry: RendererContext_GetDeviceContext()->GSSetConstantBuffers(decl->Slot, 1, &constantBuffer->Buffer); break;
	case ShaderType_Pixel: RendererContext_GetDeviceContext()->PSSetConstantBuffers(decl->Slot, 1, &constantBuffer->Buffer); break;
	case ShaderType_Compute: RendererContext_GetDeviceContext()->CSSetConstantBuffers(decl->Slot, 1, &constantBuffer->Buffer); break;
	}
}

void ConstantBuffer_Release(ConstantBuffer& constantBuffer)
{
	if (constantBuffer.Buffer)
	{
		constantBuffer.Buffer->Release();
		constantBuffer.Buffer = nullptr;
	}
}