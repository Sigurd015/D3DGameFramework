#include "pch.h"
#include "Material.h"

#define MAX_TEXTURE_SLOT 16

struct MaterialElement
{
	char* Name;
	const Texture2D* Texture;
};

void Material_Create(Material& material, const Shader& shader)
{
	material.ShaderReflectionData = Shader_GetReflectionData(shader);
	List_Create(material.Textures, MAX_TEXTURE_SLOT);
	{
		MaterialElement element;
		for (size_t i = 0; i < MAX_TEXTURE_SLOT; i++)
		{
			element.Name = nullptr;
			element.Texture = nullptr;
			List_Add(material.Textures, &element, sizeof(MaterialElement));
		}
	}
	material.TextureCount = 0;
}

void Material_Release(Material& material)
{
	List_Free(material.Textures, true);
}

void Material_SetTexture(Material& material, const char* name, const Texture2D* texture)
{
	MaterialElement* element = (MaterialElement*)List_Get(material.Textures, material.TextureCount);
	element->Name = strdup(name);
	element->Texture = texture;
	material.TextureCount++;
}

void Material_Bind(const Material& material)
{
	for (size_t i = 0; i < material.TextureCount; i++)
	{
		MaterialElement* element = (MaterialElement*)List_Get(material.Textures, i);

		HashNode* it = HashMap_Find(material.ShaderReflectionData, element->Name);
		if (it != HashMapEnd)
		{
			uint32_t bindingSlot = *(uint32_t*)it->Value;
			Texture2D_Bind(element->Texture, bindingSlot);
		}
	}
}

void Material_Clear(Material& material)
{
	material.TextureCount = 0;
}