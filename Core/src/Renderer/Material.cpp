#include "pch.h"
#include "Material.h"
#include "Asset/AssetManager.h"

static const char* s_AlbedoMap = "u_AlbedoTex";
static const char* s_NormalMap = "u_NormalTex";
static const char* s_MetallicRoughnessMap = "u_MetallicRoughnessTex";

struct MaterialElement
{
	char* Name;
	const Texture2D* Texture;
};

// Notice: Material used for both Renderer2D and SceneRenderer
// Renderer2D passed Renderer2D_Quad shader, SceneRenderer passed DeferredGeometry shader
// For not passing shader to Material_Create, use DeferredGeometry shader as default PBR material
void Material_Create(Material* material, const Shader* shader)
{
	if (shader)
		material->ShaderReflectionData = Shader_GetReflectionData(shader);
	else
		material->ShaderReflectionData = Shader_GetReflectionData((Shader*)AssetManager_GetAsset("DeferredGeometry", AssetType_Shader));

	List_Create(material->Textures, sizeof(MaterialElement));
}

void Material_Release(Material* material)
{
	// Free string in reflection data, list can't free it, because it doesn't know the type of the data
	List_Foreach(material->Textures, [](void* data)
		{
			MaterialElement* element = (MaterialElement*)data;
			String_Free(element->Name);
		});

	List_Free(material->Textures);
}

void Material_SetTexture(Material* material, const char* name, const Texture2D* texture)
{
	MaterialElement element;
	element.Name = String_Duplicate(name);
	element.Texture = texture;
	List_Add(material->Textures, &element);
}

void Material_Bind(const Material* material)
{
	for (size_t i = 0; i < List_Size(material->ShaderReflectionData); i++)
	{
		ShaderResourceDeclaration* decl = (ShaderResourceDeclaration*)List_Get(material->ShaderReflectionData, i);

		if (decl->ResourceType != ShaderResourceType_Resource)
			continue;

		for (size_t i = 0; i < List_Size(material->Textures); i++)
		{
			MaterialElement* element = (MaterialElement*)List_Get(material->Textures, i);

			if (String_Compare(element->Name, decl->Name))
			{
				Texture2D_Bind(element->Texture, decl);
				break;
			}
		}
	}
}

void Material_Clear(Material* material)
{
	// Free string in the data, list can't free it, because it doesn't know the type of the data
	List_Foreach(material->Textures, [](void* data)
		{
			MaterialElement* element = (MaterialElement*)data;
			String_Free(element->Name);
		});

	List_Clear(material->Textures);
}

void Material_SetAlbedo(Material* material, const Vec3& albedo)
{
	material->Albedo = albedo;
}

void Material_SetEmission(Material* material, float emission)
{
	material->Emission = emission;
}

void Material_SetMetallic(Material* material, float metallic)
{
	material->Metallic = metallic;
}

void Material_SetRoughness(Material* material, float roughness)
{
	material->Roughness = roughness;
}

void Material_SetUseNormalMap(Material* material, bool useNormalMap)
{
	material->UseNormalMap = useNormalMap;
}

void Material_SetNormalMap(Material* material, const Texture2D* texture)
{
	MaterialElement element;
	element.Name = String_Duplicate(s_NormalMap);
	element.Texture = texture;
	List_Add(material->Textures, &element);
}

void Material_SetMetallicRoughnessMap(Material* material, const Texture2D* texture)
{
	MaterialElement element;
	element.Name = String_Duplicate(s_MetallicRoughnessMap);
	element.Texture = texture;
	List_Add(material->Textures, &element);
}

void Material_SetAlbedoMap(Material* material, const Texture2D* texture)
{
	MaterialElement element;
	element.Name = String_Duplicate(s_AlbedoMap);
	element.Texture = texture;
	List_Add(material->Textures, &element);
}

Vec3 Material_GetAlbedo(const Material* material)
{
	return material->Albedo;
}

float Material_GetEmission(const Material* material)
{
	return material->Emission;
}

float Material_GetMetallic(const Material* material)
{
	return material->Metallic;
}

float Material_GetRoughness(const Material* material)
{
	return material->Roughness;
}

bool Material_GetUseNormalMap(const Material* material)
{
	return material->UseNormalMap;
}