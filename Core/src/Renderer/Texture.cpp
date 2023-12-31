#include "pch.h"
#include "DXTrace.h"
#include "RendererContext.h"
#include "Texture.h"
#include "RendererAPI.h"

//---------------------------------------------------------------------------------------
//-----------------------------------Texture---------------------------------------------
//---------------------------------------------------------------------------------------

void Texture2D_Create(Texture2D* texture2D, const TextureSpecification& spec, Buffer data)
{
	texture2D->Spec = spec;

	ImageSpecification imageSpec = {};
	imageSpec.Width = spec.Width;
	imageSpec.Height = spec.Height;
	imageSpec.Format = spec.Format;
	imageSpec.Usage = ImageUsage_Texture2D;
	imageSpec.Mips = spec.GenerateMips ? 0 : 1;

	CORE_ASSERT(data.Data, "Texture2D_Create: Data is null!");

	Image2D_Create(&texture2D->Image, imageSpec, data);

	if (spec.GenerateMips)
	{
		RendererContext_GetDeviceContext()->GenerateMips(Image2D_GetTextureSRV(&texture2D->Image));
	}
}

void Texture2D_Bind(const Texture2D* texture2D, const ShaderResourceDeclaration* decl)
{
	Image2D_Bind(&texture2D->Image, decl);
}

bool Texture2D_IsSame(const Texture2D* texture2D, const Texture2D* other)
{
	ID3D11Resource* resource1;
	ID3D11Resource* resource2;
	Image2D_GetTextureSRV(&texture2D->Image)->GetResource(&resource1);
	Image2D_GetTextureSRV(&other->Image)->GetResource(&resource2);
	return resource1 == resource2;
}

ImageFormat Texture2D_GetFormat(const Texture2D* texture2D)
{
	return texture2D->Spec.Format;
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
	Image2D_Release(&texture2D->Image);
}

//---------------------------------------------------------------------------------------
//-----------------------------------TextureCube-----------------------------------------
//---------------------------------------------------------------------------------------

void TextureCube_Create(TextureCube* textureCube, const TextureSpecification& spec, Buffer buffer)
{
	textureCube->Spec = spec;
	textureCube->ImageData = buffer;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = textureCube->Spec.Width;
	textureDesc.Height = textureCube->Spec.Height;
	textureDesc.ArraySize = 6;
	textureDesc.Format = Image2D_ImageFormatToDXTextureFormat(textureCube->Spec.Format);
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceView = {};
	resourceView.Format = textureDesc.Format;
	resourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	resourceView.Texture2D.MostDetailedMip = 0;

	if (textureCube->Spec.GenerateMips)
	{
		textureDesc.MipLevels = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		resourceView.Texture2D.MipLevels = -1;
	}
	else
	{
		textureDesc.MipLevels = 1;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		resourceView.Texture2D.MipLevels = 1;
	}

	if (textureCube->ImageData.Data)
	{
		uint32_t faceSize = textureCube->ImageData.Size / 6;
		D3D11_SUBRESOURCE_DATA subresourceData[6] = {};
		for (size_t i = 0; i < 6; i++)
		{
			subresourceData[i].pSysMem = textureCube->ImageData.Data + i * faceSize;
			subresourceData[i].SysMemPitch = textureCube->Spec.Width * 4;  // size of one row in bytes
		}
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateTexture2D(&textureDesc, subresourceData, &textureCube->Texture));

		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateShaderResourceView(textureCube->Texture, &resourceView, &textureCube->TextureSRV));

		if (textureCube->Spec.GenerateMips)
		{
			RendererContext_GetDeviceContext()->GenerateMips(textureCube->TextureSRV);
		}
	}
	else
	{
		textureDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateTexture2D(&textureDesc, nullptr, &textureCube->Texture));

		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateShaderResourceView(textureCube->Texture, &resourceView, &textureCube->TextureSRV));
	}
}

void TextureCube_Bind(const TextureCube* textureCube, const ShaderResourceDeclaration* decl)
{
	switch (decl->Stage)
	{
	case ShaderType_Vertex:
		RendererContext_GetDeviceContext()->VSSetShaderResources(decl->Slot, 1, &textureCube->TextureSRV);
		break;
	case ShaderType_Geometry:
		RendererContext_GetDeviceContext()->GSSetShaderResources(decl->Slot, 1, &textureCube->TextureSRV);
		break;
	case ShaderType_Pixel:
		RendererContext_GetDeviceContext()->PSSetShaderResources(decl->Slot, 1, &textureCube->TextureSRV);
		break;
	case ShaderType_Compute:
	{
		switch (decl->ResourceType)
		{
		case ShaderResourceType_Resource:
			RendererContext_GetDeviceContext()->CSSetShaderResources(decl->Slot, 1, &textureCube->TextureSRV);
			break;
		case ShaderResourceType_UnorderedAccess:
			RendererContext_GetDeviceContext()->CSSetUnorderedAccessViews(decl->Slot, 1, &textureCube->TextureUAV, 0);
			break;
		}
		break;
	}
	}
}

void TextureCube_Release(TextureCube* textureCube)
{
	if (textureCube->Texture)
	{
		textureCube->Texture->Release();
		textureCube->Texture = nullptr;
	}
	if (textureCube->TextureSRV)
	{
		textureCube->TextureSRV->Release();
		textureCube->TextureSRV = nullptr;
	}
	if (textureCube->TextureUAV)
	{
		textureCube->TextureUAV->Release();
		textureCube->TextureUAV = nullptr;
	}
}

uint32_t TextureCube_GetWidth(const TextureCube* textureCube)
{
	return textureCube->Spec.Width;
}

uint32_t TextureCube_GetHeight(const TextureCube* textureCube)
{
	return textureCube->Spec.Height;
}

/// <summary>
/// Notice: Default mipSlice is 0, which means the first mip level
/// When call this function at 0 mipSlice, make sure call GenerateMips() after that to generate mips
/// Call this function at other mipSlice, means manually generate mips, like use compute shader to generate mips, no need to call GenerateMips()
/// </summary>
void TextureCube_CreateUAV(TextureCube* textureCube, uint32_t mipSlice)
{
	D3D11_TEXTURE2D_DESC desc;
	textureCube->Texture->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = desc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.MipSlice = mipSlice;
	uavDesc.Texture2DArray.FirstArraySlice = 0;
	uavDesc.Texture2DArray.ArraySize = desc.ArraySize;

	RendererContext_GetDevice()->CreateUnorderedAccessView(textureCube->Texture, &uavDesc, &textureCube->TextureUAV);
}

void TextureCube_GenerateMips(TextureCube* textureCube)
{
	RendererContext_GetDeviceContext()->GenerateMips(textureCube->TextureSRV);
}

ID3D11Texture2D* TextureCube_GetTexture(const TextureCube* textureCube)
{
	return textureCube->Texture;
}

ID3D11ShaderResourceView* TextureCube_GetSRV(const TextureCube* textureCube)
{
	return textureCube->TextureSRV;
}

ID3D11UnorderedAccessView* TextureCube_GetUAV(const TextureCube* textureCube)
{
	return textureCube->TextureUAV;
}
