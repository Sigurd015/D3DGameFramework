#include "pch.h"
#include "Material.h"

void Material_Create(Material& material, const Shader& shader)
{
	HashMap_Create(material.Textures);
	material.ShaderReflectionData = Shader_GetReflectionData(shader);
}

void Material_Release(Material& material)
{
	HashMap_Free(material.Textures, false);
}

void Material_SetTexture(Material& material, const char* name, const Texture2D* texture)
{
	HashMap_Set(material.Textures, name, (void*)texture);
}

void Material_Bind(const Material& material)
{
	for (size_t i = 0; i < HashMap_GetTableSize(); i++)
	{
		HashNode* currentNode = HashMap_Get(material.Textures, i);
		if (currentNode == nullptr)
		{
			continue;
		}

		while (currentNode != nullptr)
		{
			HashNode* it = HashMap_Find(material.ShaderReflectionData, currentNode->Key);
			if (it != HashMapEnd)
			{
				uint32_t bindingSlot = *(uint32_t*)it->Value;
				Texture2D_Bind((Texture2D*)currentNode->Value, bindingSlot);
			}
			currentNode = currentNode->Next;
		}
	}
}