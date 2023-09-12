#include "pch.h"
#include "DXTrace.h"
#include "RendererContext.h"
#include "Texture.h"
#include "RendererAPI.h"

#include <WICTextureLoader.h>

DXGI_FORMAT ImageFormatToDXDataFormat(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::RGB8:  return DXGI_FORMAT_R8G8B8A8_UNORM;
	case ImageFormat::RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	CORE_ASSERT(false, "Unknown ImageFormat!");
	return DXGI_FORMAT_UNKNOWN;
}

void CreateTexture(D3D11_USAGE usage, int cpuAccess, uint32_t width, uint32_t height,
	DXGI_FORMAT format, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D)
{
	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = usage;
	textureDesc.CPUAccessFlags = cpuAccess;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = 0;
	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateTexture2D(&textureDesc, pInitialData, ppTexture2D));
}

void CreateShaderView(DXGI_FORMAT format, ID3D11Resource* pResource, ID3D11ShaderResourceView** ppSRView)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceView = {};
	resourceView.Format = format;
	resourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceView.Texture2D.MostDetailedMip = 0;
	resourceView.Texture2D.MipLevels = 1;
	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateShaderResourceView(pResource, &resourceView, ppSRView));
}

void CreateSamplerState(ID3D11SamplerState** ppSamplerState)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateSamplerState(&samplerDesc, ppSamplerState));
}

void Texture2D_Create(Texture2D* out, const char* path)
{
	size_t newsize = strlen(path) + 1;

	wchar_t* wcstring = new wchar_t[newsize];

	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, path, _TRUNCATE);

	CORE_CHECK_DX_RESULT(DirectX::CreateWICTextureFromFileEx(RendererContext_GetDevice(), wcstring, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB, reinterpret_cast<ID3D11Resource**>(&out->Texture), &out->TextureView));

	delete[]wcstring;

	D3D11_TEXTURE2D_DESC desc;
	out->Texture->GetDesc(&desc);
	out->DataFormat = desc.Format;
	out->Spec.Width = desc.Width;
	out->Spec.Height = desc.Height;
	out->Spec.Format = ImageFormat::RGBA8;

	CreateSamplerState(&out->SamplerState);
}

void Texture2D_Create(Texture2D* out, const TextureSpecification& spec)
{
	out->Spec = spec;
	out->DataFormat = ImageFormatToDXDataFormat(spec.Format);
	CreateTexture(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, spec.Width, spec.Height, out->DataFormat, nullptr, &out->Texture);
	CreateShaderView(out->DataFormat, out->Texture, &out->TextureView);
	CreateSamplerState(&out->SamplerState);
}

void Texture2D_SetData(Texture2D* out, void* data, uint32_t size)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	CORE_CHECK_DX_RESULT(RendererContext_GetDeviceContext()->Map(out->Texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	if (out->Spec.Format == ImageFormat::RGB8)
	{
		uint8_t* targetData = static_cast<uint8_t*>(mappedResource.pData);
		uint8_t* srcData = static_cast<uint8_t*>(data);

		// Set Alpha to 255	
		for (uint32_t i = 0; i < size / 3; i++)
		{
			targetData[i * 4] = srcData[i * 3];     // R
			targetData[i * 4 + 1] = srcData[i * 3 + 1]; // G
			targetData[i * 4 + 2] = srcData[i * 3 + 2]; // B
			targetData[i * 4 + 3] = 255;           // Alpha
		}
	}
	else
		memcpy(mappedResource.pData, data, size);

	RendererContext_GetDeviceContext()->Unmap(out->Texture, 0);
}

void Texture2D_Bind(const Texture2D* out, uint32_t slot)
{
	RendererContext_GetDeviceContext()->PSSetSamplers(slot, 1, &out->SamplerState);
	RendererContext_GetDeviceContext()->PSSetShaderResources(slot, 1, &out->TextureView);
}

bool Texture2D_IsSame(const Texture2D* out, const Texture2D* other)
{
	ID3D11Resource* resource1;
	ID3D11Resource* resource2;
	out->TextureView->GetResource(&resource1);
	other->TextureView->GetResource(&resource2);
	return resource1 == resource2;
}

uint32_t Texture2D_GetWidth(const Texture2D* out)
{
	return out->Spec.Width;
}

uint32_t Texture2D_GetHeight(const Texture2D* out)
{
	return out->Spec.Height;
}

void Texture2D_Release(Texture2D* out)
{
	if (out->Texture)
	{
		out->Texture->Release();
		out->Texture = nullptr;
	}
	if (out->TextureView)
	{
		out->TextureView->Release();
		out->TextureView = nullptr;
	}
	if (out->SamplerState)
	{
		out->SamplerState->Release();
		out->SamplerState = nullptr;
	}
}