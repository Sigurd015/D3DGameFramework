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
	List_Create(material.Textures, sizeof(MaterialElement), MAX_TEXTURE_SLOT);
}

void Material_Release(Material& material)
{
	List_Free(material.Textures);
}

void Material_SetTexture(Material& material, const char* name, const Texture2D* texture)
{
	MaterialElement element;
	element.Name = strdup(name);
	element.Texture = texture;
	List_Add(material.Textures, &element);
}

void Material_Bind(const Material& material)
{
	for (size_t i = 0; i < List_Size(material.ShaderReflectionData); i++)
	{
		ShaderResourceDeclaration* decl = (ShaderResourceDeclaration*)List_Get(material.ShaderReflectionData, i);

		for (size_t i = 0; i < List_Size(material.Textures); i++)
		{
			MaterialElement* element = (MaterialElement*)List_Get(material.Textures, i);

			if (strcmp(element->Name, decl->Name) == 0)
			{
				Texture2D_Bind(element->Texture, decl);
				break;
			}
		}
	}
}

void Material_Clear(Material& material)
{
	List_Clear(material.Textures);
}