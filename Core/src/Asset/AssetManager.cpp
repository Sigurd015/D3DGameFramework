#include "pch.h"
#include "AssetManager.h"
#include "Audio/Audio.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Audio/Audio.h"

enum AssetType
{
	AssetType_Unknown,
	AssetType_Texture,
	AssetType_Mesh,
	AssetType_Sound,
	AssetType_Shader,
	AssetType_Count
};

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

void* AssetManager_GetAsset(const char* assetPath, bool isMemoryAsset)
{
	void* result = HashMap_Find(s_Assets, assetPath);
	if (result)
	{
		AssetElement* element = (AssetElement*)result;
		return element->Asset;
	}

	if (isMemoryAsset)
	{
		// TODO: Now we only support shader as memory asset
		AssetElement element;
		element.Type = AssetType_Shader;
		element.Asset = Shader_Create(assetPath);
		HashMap_Set(s_Assets, assetPath, &element);
		return element.Asset;
	}

	AssetElement element;
	switch (GetAssetType(assetPath))
	{
	case AssetType_Texture:
	{
		element.Type = AssetType_Texture;
		element.Asset = Texture2D_Create(assetPath);
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
				break;
			}
			}
		});

	HashMap_Free(s_Assets);
}