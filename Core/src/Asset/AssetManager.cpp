#include "pch.h"
#include "AssetManager.h"
#include "Renderer/Texture.h"
#include "Audio/Audio.h"

enum AssetType
{
	AssetType_Unknown,
	AssetType_Texture,
	AssetType_Mesh,
	AssetType_Sound,
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
		if (strstr(assetPath, s_Extensions[i].Extension) != NULL)
		{
			return s_Extensions[i].Type;
		}
	}

	return AssetType_Unknown;
}

void* AssetManager_GetAsset(const char* assetPath)
{
	HashNode* it = HashMap_Find(s_Assets, assetPath);
	if (it != HashMapEnd)
	{
		AssetElement* element = (AssetElement*)it->Value;
		return element->Asset;
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
	HashMap_Set(s_Assets, assetPath, &element, sizeof(AssetElement));
	return element.Asset;
}

void AssetManager_Init()
{
	HashMap_Create(s_Assets);
}

void AssetManager_Shutdown()
{
	for (size_t i = 0; i < HashMap_GetTableSize(); i++)
	{
		HashNode* currentNode = HashMap_Get(s_Assets, i);
		if (currentNode == nullptr)
		{
			continue;
		}

		while (currentNode != nullptr)
		{
			AssetElement* element = (AssetElement*)currentNode->Value;
			switch (element->Type)
			{
			case AssetType_Texture:
			{
				Texture2D_Release((Texture2D*)element->Asset);
				break;
			}
			case AssetType_Sound:
			{
				delete element->Asset;
				break;
			}
			}
			currentNode = currentNode->Next;
		}
	}
	HashMap_Free(s_Assets, true);
}