#pragma once
#include "Renderer/Texture.h"

enum AssetType
{
	AssetType_Unknown,
	AssetType_Texture,
	AssetType_TextureCube,
	AssetType_EnvMap,
	AssetType_Mesh,
	AssetType_Sound,
	AssetType_Shader,
	AssetType_Count
};

void AssetManager_Init();
void AssetManager_Shutdown();

// You can pass in a path to a texture, mesh, or sound, 
// alos you can pass in a shader name, and set the type to AssetType_Shader
// optionalData is passed type specific data,
struct TextureCreationOptionalData
{
	TextureSpecification Spec;
	Buffer TextureData;
};
void* AssetManager_GetAsset(const char* assetPath, AssetType type = AssetType_Unknown, void* optionalData = nullptr);