#include "pch.h"
#include "DXTrace.h"
#include "RendererContext.h"
#include "Texture.h"
#include "RendererAPI.h"

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

	Image2D_Create(texture2D->Image, imageSpec, data);

	if (spec.GenerateMips)
	{
		RendererContext_GetDeviceContext()->GenerateMips(Image2D_GetTextureSRV(texture2D->Image));
	}
}

void Texture2D_Bind(const Texture2D* texture2D, const ShaderResourceDeclaration* decl)
{
	Image2D_Bind(texture2D->Image, decl);
}

bool Texture2D_IsSame(const Texture2D* texture2D, const Texture2D* other)
{
	ID3D11Resource* resource1;
	ID3D11Resource* resource2;
	Image2D_GetTextureSRV(texture2D->Image)->GetResource(&resource1);
	Image2D_GetTextureSRV(other->Image)->GetResource(&resource2);
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
	Image2D_Release(texture2D->Image);
}