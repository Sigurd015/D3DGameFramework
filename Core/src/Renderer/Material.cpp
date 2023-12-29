#include "pch.h"
#include "Material.h"

struct MaterialElement
{
	char* Name;
	const Texture2D* Texture;
};

void Material_Create(Material& material, const Shader* shader)
{
	material.ShaderReflectionData = Shader_GetReflectionData(shader);
	List_Create(material.Textures, sizeof(MaterialElement));
}

void Material_Release(Material& material)
{
	// Free string in reflection data, list can't free it, because it doesn't know the type of the data
	List_Foreach(material.Textures, [](void* data)
		{
			MaterialElement* element = (MaterialElement*)data;
			String_Free(element->Name);
		});

	List_Free(material.Textures);
}

void Material_SetTexture(Material& material, const char* name, const Texture2D* texture)
{
	MaterialElement element;
	element.Name = String_Duplicate(name);
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

			if (String_Compare(element->Name, decl->Name))
			{
				Texture2D_Bind(element->Texture, decl);
				break;
			}
		}
	}
}

void Material_Clear(Material& material)
{
	// Free string in the data, list can't free it, because it doesn't know the type of the data
	List_Foreach(material.Textures, [](void* data)
		{
			MaterialElement* element = (MaterialElement*)data;
			String_Free(element->Name);
		});

	List_Clear(material.Textures);
}