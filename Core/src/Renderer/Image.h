#pragma once
#include "DXTrace.h"
#include "RendererContext.h"
#include "Shader.h"
#include "Core/Buffer.h"

#include <stdint.h>
#include <d3d11.h>

enum ImageFormat
{
	ImageFormat_None = 0,
	ImageFormat_RGBA,
	ImageFormat_RGBA8F,
	ImageFormat_RGBA16F,
	ImageFormat_RGBA32F,

	// Depth/stencil
	ImageFormat_DEPTH24STENCIL8,
	ImageFormat_DEPTH32F,

	ImageFormat_SRGB,

	// Defaults
	ImageFormat_Depth = ImageFormat_DEPTH24STENCIL8,
	ImageFormat_ShadowMap = ImageFormat_DEPTH32F
};

enum ImageUsage
{
	ImageUsage_None = 0,
	ImageUsage_Texture2D,
	ImageUsage_Attachment,
};

struct ImageSpecification
{
	ImageFormat Format = ImageFormat_RGBA;
	ImageUsage Usage = ImageUsage_Texture2D;
	uint32_t Width = 1;
	uint32_t Height = 1;
	uint32_t Mips = 1;
	uint32_t Layers = 1;
};

struct Image2D
{
	ImageSpecification Specification;
	DXGI_FORMAT DataFormat;
	ID3D11Texture2D* Texture = nullptr;
	ID3D11ShaderResourceView* TextureSRV = nullptr;

	Buffer ImageData;
};

void Image2D_Create(Image2D& image2D, const ImageSpecification& spec, Buffer buffer = { nullptr,0 });
void Image2D_Release(Image2D& image2D);
void Image2D_Bind(const Image2D& image2D, const ShaderResourceDeclaration* decl);
void* Image2D_GetRendererID(const Image2D& image2D);
uint32_t Image2D_GetWidth(const Image2D& image2D);
uint32_t Image2D_GetHeight(const Image2D& image2D);
DXGI_FORMAT Image2D_GetDXGIFormat(const Image2D& image2D);
ID3D11Texture2D* Image2D_GetTexture(const Image2D& image2D);
ID3D11ShaderResourceView* Image2D_GetTextureSRV(const Image2D& image2D);
const ImageSpecification& Image2D_GetSpecification(const Image2D& image2D);

bool Image2D_IsDepthFormat(ImageFormat format);
DXGI_FORMAT Image2D_ImageFormatToDXDepthDSVFormat(ImageFormat format);