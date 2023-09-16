#pragma once

enum RendererResourceType
{
	RendererResourceType_Unknown = 0,
	RendererResourceType_ConstantBuffer,
	RendererResourceType_Texture2D,
	RendererResourceType_TextureCube,
	RendererResourceType_Count
};

struct RendererResource
{
	RendererResourceType Type = RendererResourceType::RendererResourceType_Unknown;
	void* Data = nullptr;
};

void RendererResource_Create(RendererResource& rendererResource, RendererResourceType type, void* data);