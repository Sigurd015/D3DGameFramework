#include "pch.h"
#include "RendererResource.h"
#include "ConstantBuffer.h"
#include "Texture.h"

void RendererResource_Create(RendererResource& rendererResource, RendererResourceType type, void* data)
{
	rendererResource.Type = type;
	rendererResource.Data = data;
}
