#include "pch.h"
#include "Image.h"

// UNORM: Unsigned normalized
// SNORM: Signed normalized
// FLOAT: Signed float
DXGI_FORMAT ImageFormatToDXTextureFormat(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat_SRGB:            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	case ImageFormat_RGBA:            return DXGI_FORMAT_R8G8B8A8_UNORM;
	case ImageFormat_RGBA16F:         return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case ImageFormat_RGBA32F:         return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case ImageFormat_DEPTH24STENCIL8: return DXGI_FORMAT_R24G8_TYPELESS;
	case ImageFormat_DEPTH32F:        return DXGI_FORMAT_R32_TYPELESS;
	}

	CORE_ASSERT(false, "Unknown ImageFormat!");
	return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT ImageFormatToDXDepthSRVFormat(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat_DEPTH24STENCIL8: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case ImageFormat_DEPTH32F: return DXGI_FORMAT_R32_FLOAT;
	}

	CORE_ASSERT(false, "Unknown ImageFormat!");
	return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT Image2D_ImageFormatToDXDepthDSVFormat(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat_DEPTH24STENCIL8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case ImageFormat_DEPTH32F: return DXGI_FORMAT_D32_FLOAT;
	}

	CORE_ASSERT(false, "Unknown ImageFormat!");
	return DXGI_FORMAT_UNKNOWN;
}

bool Image2D_IsDepthFormat(ImageFormat format)
{
	if (format == ImageFormat_DEPTH24STENCIL8 || format == ImageFormat_DEPTH32F)
		return true;

	return false;
}

DXGI_SAMPLE_DESC Multisample(UINT count, DXGI_FORMAT format)
{
	DXGI_SAMPLE_DESC sampleDesc{};
	sampleDesc.Count = count;
	if (count > 1)
	{
		UINT  m_Quality;
		RendererContext_GetDevice()->CheckMultisampleQualityLevels(format, count, &m_Quality);
		sampleDesc.Quality = m_Quality - 1;
	}
	else
	{
		sampleDesc.Quality = 0;
	}
	return sampleDesc;
}

void Image2D_Create(Image2D& image2D, const ImageSpecification& spec, Buffer buffer)
{
	image2D.Specification = spec;

	image2D.DataFormat = ImageFormatToDXTextureFormat(spec.Format);
	image2D.ImageData = buffer;

	switch (image2D.Specification.Usage)
	{
	case ImageUsage_Attachment:
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = image2D.Specification.Width;
		textureDesc.Height = image2D.Specification.Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = image2D.Specification.Layers;
		textureDesc.Format = image2D.DataFormat;
		// TODO: Implement multisampling
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.CPUAccessFlags = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc = {};

		if (textureDesc.ArraySize > 1)
		{
			if (textureDesc.ArraySize == 6)
			{
				textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
				shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				shaderResourceDesc.TextureCube.MipLevels = 1;
			}
			else
			{
				shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				shaderResourceDesc.Texture2DArray.MipLevels = 1;
			}
		}
		else
		{
			shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceDesc.Texture2D.MipLevels = 1;
		}

		if (Image2D_IsDepthFormat(image2D.Specification.Format))
		{
			// If it's a depth attachment, we need to create a depth stencil view and usa a depth stencil format
			textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

			shaderResourceDesc.Format = ImageFormatToDXDepthSRVFormat(image2D.Specification.Format);
		}
		else
		{
			// If it's a color attachment, we need to create a render target view and use a regular texture format
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

			shaderResourceDesc.Format = textureDesc.Format;
		}
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateTexture2D(&textureDesc, nullptr, &image2D.Texture));

		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateShaderResourceView(image2D.Texture, &shaderResourceDesc, &image2D.TextureSRV));
		break;
	}
	case ImageUsage_Texture2D:
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = image2D.Specification.Width;
		textureDesc.Height = image2D.Specification.Height;
		textureDesc.MipLevels = image2D.Specification.Mips;
		textureDesc.ArraySize = 1;
		textureDesc.Format = image2D.DataFormat;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.CPUAccessFlags = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceView = {};
		resourceView.Format = image2D.DataFormat;
		resourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceView.Texture2D.MostDetailedMip = 0;

		// Notice: Must sure load image data as RGBA 4 channels
		const static uint32_t defalutDataSize = 4;
		if (image2D.Specification.Mips != 1)
		{
			// If we have mipmaps, we need to create the texture without data and then update the first mip level with the data
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
			CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateTexture2D(&textureDesc, nullptr, &image2D.Texture));
			RendererContext_GetDeviceContext()->UpdateSubresource(image2D.Texture, 0, nullptr,
				image2D.ImageData.Data, image2D.Specification.Width * defalutDataSize, 0);

			resourceView.Texture2D.MipLevels = -1;
		}
		else
		{
			// If we have no mipmaps, we can directly create the texture with the data
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA subresourceData = {};
			subresourceData.pSysMem = image2D.ImageData.Data;

			// Notice: RGBA32F format used for HDR image
			if (image2D.Specification.Format == ImageFormat_RGBA32F)
				subresourceData.SysMemPitch = image2D.Specification.Width * defalutDataSize * sizeof(float);  // size of one row in bytes
			else
				subresourceData.SysMemPitch = image2D.Specification.Width * defalutDataSize;  // size of one row in bytes

			CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateTexture2D(&textureDesc, &subresourceData, &image2D.Texture));

			resourceView.Texture2D.MipLevels = 1;
		}

		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateShaderResourceView(image2D.Texture, &resourceView, &image2D.TextureSRV));
		break;
	}
	}
}

void Image2D_Release(Image2D& image2D)
{
	if (image2D.Texture)
	{
		image2D.Texture->Release();
		image2D.Texture = nullptr;
	}

	if (image2D.TextureSRV)
	{
		image2D.TextureSRV->Release();
		image2D.TextureSRV = nullptr;
	}
}

void Image2D_Bind(const Image2D& image2D, const ShaderResourceDeclaration* decl)
{
	switch (decl->Stage)
	{
	case ShaderType_Vertex: RendererContext_GetDeviceContext()->VSSetShaderResources(decl->Slot, 1, &image2D.TextureSRV); break;
	case ShaderType_Geometry: RendererContext_GetDeviceContext()->GSSetShaderResources(decl->Slot, 1, &image2D.TextureSRV); break;
	case ShaderType_Pixel: RendererContext_GetDeviceContext()->PSSetShaderResources(decl->Slot, 1, &image2D.TextureSRV); break;
	case ShaderType_Compute: RendererContext_GetDeviceContext()->CSSetShaderResources(decl->Slot, 1, &image2D.TextureSRV); break;
	}
}

void* Image2D_GetRendererID(const Image2D& image2D)
{
	return image2D.TextureSRV;
}

uint32_t Image2D_GetWidth(const Image2D& image2D)
{
	return image2D.Specification.Width;
}

uint32_t Image2D_GetHeight(const Image2D& image2D)
{
	return image2D.Specification.Height;
}

DXGI_FORMAT Image2D_GetDXGIFormat(const Image2D& image2D)
{
	return image2D.DataFormat;
}

ID3D11Texture2D* Image2D_GetTexture(const Image2D& image2D)
{
	return image2D.Texture;
}

ID3D11ShaderResourceView* Image2D_GetTextureSRV(const Image2D& image2D)
{
	return image2D.TextureSRV;
}

const ImageSpecification& Image2D_GetSpecification(const Image2D& image2D)
{
	return image2D.Specification;
}