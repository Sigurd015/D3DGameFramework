#include "pch.h"
#include "AssetManager.h"
#include "Audio/Audio.h"
#include "Renderer/Shader.h"
#include "Audio/Audio.h"
#include "TextureLoader.h"

struct AssetExtensions
{
	const char* Extension;
	AssetType Type;
};

#define MAX_ASSET_EXTENSIONS_COUNT 10

static AssetExtensions s_Extensions[MAX_ASSET_EXTENSIONS_COUNT] =
{
	{ ".png",  AssetType_Texture },
	{ ".jpg",  AssetType_Texture },
	{ ".jpeg", AssetType_Texture },
	{ ".tga",  AssetType_Texture },
	{ ".hdr",  AssetType_Texture },

	{ ".obj", AssetType_Mesh },
	{ ".fbx", AssetType_Mesh },
	{ ".gltf", AssetType_Mesh },

	{ ".mp3", AssetType_Sound },
	{ ".wav", AssetType_Sound },
};

struct AssetElement
{
	AssetType Type;
	void* Asset;
};

static HashMap s_Assets;

static AssetType GetAssetType(const char* assetPath)
{
	for (int i = 0; i < MAX_ASSET_EXTENSIONS_COUNT; i++)
	{
		if (String_Contains(assetPath, s_Extensions[i].Extension))
		{
			return s_Extensions[i].Type;
		}
	}

	return AssetType_Unknown;
}

void* AssetManager_GetAsset(const char* assetPath, AssetType type, void* optionalData)
{
	void* result = HashMap_Find(s_Assets, assetPath);
	if (result)
	{
		AssetElement* element = (AssetElement*)result;
		return element->Asset;
	}

	AssetElement element;

	// Memory only assets, assetPath is the name of the asset
	// Shader is kind of a special case, use the name of the shader to load it, not the path
	// So use Memory only assets for shaders
	if (type != AssetType_Unknown)
	{
		switch (type)
		{
		case AssetType_Texture:
		{
			if (!optionalData)
			{
				CORE_LOG_ERROR("AssetManager_GetAsset: optionalData is null");
				return nullptr;
			}
			TextureCreationOptionalData* data = (TextureCreationOptionalData*)optionalData;
			element.Type = AssetType_Texture;
			element.Asset = Memory_Allocate(sizeof(Texture2D), MemoryBlockTag_Texture);
			Texture2D_Create((Texture2D*)element.Asset, data->Spec, data->TextureData);
			break;
		}
		case AssetType_Shader:
		{
			element.Type = AssetType_Shader;
			element.Asset = Memory_Allocate(sizeof(Shader), MemoryBlockTag_Shader);
			Shader_Create((Shader*)element.Asset, assetPath);
			break;
		}
		}
		HashMap_Set(s_Assets, assetPath, &element);
		return element.Asset;
	}

	switch (GetAssetType(assetPath))
	{
	case AssetType_Texture:
	{
		element.Type = AssetType_Texture;
		element.Asset = Memory_Allocate(sizeof(Texture2D), MemoryBlockTag_Texture);

		bool result = TextureLoader_TryLoad((Texture2D*)element.Asset, assetPath);
		if (!result)
		{
			CORE_LOG_ERROR("AssetManager_GetAsset: Failed to load texture from file: %s", assetPath);
			return nullptr;
		}
		break;
	}
	case AssetType_Mesh:
	{
		element.Type = AssetType_Mesh;
		// TODO: Load mesh
		break;
	}
	case AssetType_Sound:
	{
		element.Type = AssetType_Sound;
		element.Asset = Audio_CreateSoundEffect(assetPath);
		break;
	}
	}
	HashMap_Set(s_Assets, assetPath, &element);
	return element.Asset;
}

void AssetManager_Init()
{
	HashMap_Create(s_Assets, false, sizeof(AssetElement));
}

void AssetManager_Shutdown()
{
	HashMap_Foreach(s_Assets, [](void* value)
		{
			AssetElement* element = (AssetElement*)value;
			switch (element->Type)
			{
			case AssetType_Texture:
			{
				Texture2D_Release((Texture2D*)element->Asset);
				Memory_Free(element->Asset, sizeof(Texture2D), MemoryBlockTag_Texture);
				break;
			}
			case AssetType_Sound:
			{
				Audio_ReleaseSoundEffect(element->Asset);
				break;
			}
			case AssetType_Shader:
			{
				Shader_Release((Shader*)element->Asset);
				Memory_Free(element->Asset, sizeof(Shader), MemoryBlockTag_Shader);
				break;
			}
			}
		});

	HashMap_Free(s_Assets);
}