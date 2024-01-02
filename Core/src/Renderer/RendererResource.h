#pragma once
#include "Image.h"

enum RendererResourceType
{
	RendererResourceType_Unknown = 0,
	RendererResourceType_Image,
	RendererResourceType_FrameBuffer,
	RendererResourceType_ConstantBuffer,
	RendererResourceType_Texture2D,
	RendererResourceType_TextureCube,
	RendererResourceType_Count
};

struct ResourceElement
{
	char* Name;
	RendererResourceType Type;
	const void* Resource;
};

void RendererResourcePool_Init();

struct ImageCreationOptionalData
{
	ImageSpecification Spec;
	Buffer ImageData;
};
void* RendererResourcePool_GetResource(RendererResourceType type, void* optionalData);
void RendererResourcePool_Shutdown();