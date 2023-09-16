#pragma once
#include "Utils/HashMap.h"
#include "Texture.h"
#include "Shader.h"

struct Material
{
	HashMap Textures;

	HashMap ShaderReflectionData;
};

void Material_Create(Material& material, const Shader& shader);
void Material_Release(Material& material);
void Material_SetTexture(Material& material, const char* name, const Texture2D* texture);
void Material_Bind(const Material& material);