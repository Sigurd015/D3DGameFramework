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
	case ImageFormat::RGB8:  return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	case ImageFormat::RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	}

	CORE_ASSERT(false, "Unknown ImageFormat!");
	return DXGI_FORMAT_UNKNOWN;
}

void* Texture2D_Create(const char* path)
{
	size_t newsize = strlen(path) + 1;

	wchar_t* wcstring = new wchar_t[newsize];

	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, path, _TRUNCATE);

	Texture2D* texture2D = (Texture2D*)malloc(sizeof(Texture2D));
	*texture2D = {};

	CORE_CHECK_DX_RESULT(DirectX::CreateWICTextureFromFileEx(RendererContext_GetDevice(), wcstring, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_DEFAULT, reinterpret_cast<ID3D11Resource**>(&texture2D->Texture), &texture2D->TextureView));

	delete[]wcstring;

	D3D11_TEXTURE2D_DESC desc;
	texture2D->Texture->GetDesc(&desc);
	texture2D->DataFormat = desc.Format;
	texture2D->Spec.Width = desc.Width;
	texture2D->Spec.Height = desc.Height;
	texture2D->Spec.Format = ImageFormat::RGBA8;

	texture2D->NeedRelease = true;

	return texture2D;
}

void Texture2D_Create(Texture2D* texture2D, const TextureSpecification& spec)
{
	texture2D->Spec = spec;
	texture2D->DataFormat = ImageFormatToDXDataFormat(spec.Format);

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = texture2D->Spec.Width;
	textureDesc.Height = texture2D->Spec.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = texture2D->DataFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = texture2D->Spec.Data;
	subresourceData.SysMemPitch = texture2D->Spec.Width * 4;  // size of one row in bytes
	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateTexture2D(&textureDesc, &subresourceData, &texture2D->Texture));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceView = {};
	resourceView.Format = texture2D->DataFormat;
	resourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceView.Texture2D.MostDetailedMip = 0;
	resourceView.Texture2D.MipLevels = 1;
	CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateShaderResourceView(texture2D->Texture, &resourceView, &texture2D->TextureView));
}

void Texture2D_Bind(const Texture2D* texture2D, uint32_t slot)
{
	RendererContext_GetDeviceContext()->PSSetShaderResources(slot, 1, &texture2D->TextureView);
}

bool Texture2D_IsSame(const Texture2D* texture2D, const Texture2D* other)
{
	ID3D11Resource* resource1;
	ID3D11Resource* resource2;
	texture2D->TextureView->GetResource(&resource1);
	other->TextureView->GetResource(&resource2);
	return resource1 == resource2;
}

uint32_t Texture2D_GetWidth(const Texture2D* texture2D)
{
	return texture2D->Spec.Width;
}

uint32_t Texture2D_GetHeight(const Texture2D* texture2D)
{
	return texture2D->Spec.Height;
}

void Texture2D_Release(Texture2D* texture2D)
{
	if (texture2D->Texture)
	{
		texture2D->Texture->Release();
		texture2D->Texture = nullptr;
	}
	if (texture2D->TextureView)
	{
		texture2D->TextureView->Release();
		texture2D->TextureView = nullptr;
	}
	if (texture2D->NeedRelease)
	{
		free(texture2D);
	}
}