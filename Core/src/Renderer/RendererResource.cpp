#include "pch.h"
#include "RendererResource.h"
#include "FrameBuffer.h"
#include "Image.h"

static List s_RendererResources;

void RendererResourcePool_Init()
{
	List_Create(s_RendererResources, sizeof(ResourceElement));
}

void* RendererResourcePool_GetResource(RendererResourceType type, void* optionalData)
{
	switch (type)
	{
	case RendererResourceType_FrameBuffer:
	{
		FramebufferSpecification* spec = (FramebufferSpecification*)optionalData;

		Framebuffer* framebuffer = (Framebuffer*)Memory_Allocate(sizeof(Framebuffer), MemoryBlockTag_RendererResource);
		Framebuffer_Create(framebuffer, *spec);

		ResourceElement element;
		element.Name = nullptr;
		element.Type = RendererResourceType_FrameBuffer;
		element.Resource = framebuffer;
		List_Add(s_RendererResources, &element);
		return framebuffer;
	}
	case RendererResourceType_Image:
	{
		ImageCreationOptionalData* data = (ImageCreationOptionalData*)optionalData;
		ImageSpecification spec = data->Spec;
		Buffer textureData = data->ImageData;

		Image2D* image = (Image2D*)Memory_Allocate(sizeof(Image2D), MemoryBlockTag_RendererResource);

		Image2D_Create(image, spec, textureData);

		ResourceElement element;
		element.Name = nullptr;
		element.Type = RendererResourceType_Image;
		element.Resource = image;
		List_Add(s_RendererResources, &element);
		return image;
	}
	}

	CORE_ASSERT(false, "Unknown RendererResourceType!");
	return nullptr;
}

void RendererResourcePool_Shutdown()
{
	List_Foreach(s_RendererResources, [](void* data)
		{
			ResourceElement* element = (ResourceElement*)data;
			switch (element->Type)
			{
			case RendererResourceType_FrameBuffer:
			{
				Framebuffer* framebuffer = (Framebuffer*)element->Resource;
				Framebuffer_Release(framebuffer);
				break;
			}
			case RendererResourceType_Image:
			{
				Image2D* image = (Image2D*)element->Resource;
				Image2D_Release(image);
				break;
			}
			}
		});
	List_Free(s_RendererResources);
}