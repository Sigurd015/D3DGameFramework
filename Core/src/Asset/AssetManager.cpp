#include "pch.h"
#include "AssetManager.h"
#include "Audio/Audio.h"
#include "Renderer/Shader.h"
#include "Audio/Audio.h"
#include "TextureLoader.h"
#include "Renderer/EnvMap.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Mesh.h"
#include "MeshLoader.h"

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
	{ ".hdr",  AssetType_EnvMap },

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
			// AssetType_TextureCube and AssetType_EnvMap are the same
			// AssetType_TextureCube is used for memory only assets
			// AssetType_EnvMap is used for loading from hdr files
		case AssetType_TextureCube:
		{
			if (!optionalData)
			{
				CORE_LOG_ERROR("AssetManager_GetAsset: optionalData is null");
				return nullptr;
			}
			TextureCreationOptionalData* data = (TextureCreationOptionalData*)optionalData;
			element.Type = AssetType_TextureCube;
			element.Asset = Memory_Allocate(sizeof(TextureCube), MemoryBlockTag_TextureCube);
			TextureCube_Create((TextureCube*)element.Asset, data->Spec, data->TextureData);
			break;
		}
		case AssetType_Texture:
		{
			if (!optionalData)
			{
				CORE_LOG_ERROR("AssetManager_GetAsset: optionalData is null");
				return nullptr;
			}
			TextureCreationOptionalData* data = (TextureCreationOptionalData*)optionalData;
			element.Type = AssetType_Texture;
			element.Asset = Memory_Allocate(sizeof(Texture2D), MemoryBlockTag_Texture2D);
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
		case AssetType_Mesh:
		{
			element.Type = AssetType_Mesh;
			element.Asset = Memory_Allocate(sizeof(Mesh), MemoryBlockTag_Mesh);
			MeshSource* meshSource = (MeshSource*)optionalData;
			Mesh_Create((Mesh*)element.Asset, meshSource);
			break;
		}
		case AssetType_Material:
		{
			if (!optionalData)
			{
				CORE_LOG_ERROR("AssetManager_GetAsset: optionalData is null");
				return nullptr;
			}
			element.Type = AssetType_Material;
			element.Asset = Memory_Allocate(sizeof(Material), MemoryBlockTag_Material);
			Memory_Copy(element.Asset, optionalData, sizeof(Material));
		}
		}
		HashMap_Set(s_Assets, assetPath, &element);
		return element.Asset;
	}

	// If the asset is not in memory, load it from file
	switch (GetAssetType(assetPath))
	{
	case AssetType_EnvMap:
	{
		element.Type = AssetType_TextureCube;
		element.Asset = Memory_Allocate(sizeof(EnvMap), MemoryBlockTag_TextureCube);

		Texture2D hdrTexture;
		bool result = TextureLoader_TryLoad(&hdrTexture, assetPath);
		if (!result)
		{
			CORE_LOG_ERROR("AssetManager_GetAsset: Failed to load texture from file: %s", assetPath);
			return nullptr;
		}
		EnvMap envMap;
		envMap = RendererAPI_CreateEnvironmentMap(&hdrTexture);
		Memory_Copy(element.Asset, &envMap, sizeof(EnvMap));

		Texture2D_Release(&hdrTexture);
		break;
	}
	case AssetType_Texture:
	{
		element.Type = AssetType_Texture;
		element.Asset = Memory_Allocate(sizeof(Texture2D), MemoryBlockTag_Texture2D);

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
		element.Asset = Memory_Allocate(sizeof(Mesh), MemoryBlockTag_Mesh);
		MeshSource meshSource;
		MeshLoader_TryLoad(&meshSource, assetPath);
		Mesh_Create((Mesh*)element.Asset, &meshSource);
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
	HashMap_Create(s_Assets, sizeof(AssetElement));
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
				Memory_Free(element->Asset, sizeof(Texture2D), MemoryBlockTag_Texture2D);
				break;
			}
			case AssetType_TextureCube:
			{
				TextureCube_Release((TextureCube*)element->Asset);
				Memory_Free(element->Asset, sizeof(TextureCube), MemoryBlockTag_TextureCube);
				break;
			}
			case AssetType_Mesh:
			{
				Mesh_Release((Mesh*)element->Asset);
				Memory_Free(element->Asset, sizeof(Mesh), MemoryBlockTag_Mesh);
				break;
			}
			case AssetType_EnvMap:
			{
				EnvMap* envMap = (EnvMap*)element->Asset;
				TextureCube_Release(&envMap->IrradianceMap);
				TextureCube_Release(&envMap->RadianceMap);
				Memory_Free(element->Asset, sizeof(EnvMap), MemoryBlockTag_TextureCube);
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