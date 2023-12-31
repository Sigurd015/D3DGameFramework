#pragma once

enum RendererResourceType
{
	RendererResourceType_Unknown = 0,
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