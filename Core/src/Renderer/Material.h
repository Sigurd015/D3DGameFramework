#pragma once
#include "Utils/HashMap.h"
#include "Texture.h"
#include "Shader.h"

struct Material
{
	List Textures;

	List ShaderReflectionData;

	Vec3 Albedo = { 1.0f, 1.0f, 1.0f };
	float Emission = 0.0f;
	float Metallic = 0.0f;
	float Roughness = 0.0f;
	bool UseNormalMap = false;
};

void Material_Create(Material* material, const Shader* shader = nullptr);
void Material_Release(Material* material);
void Material_SetTexture(Material* material, const char* name, const Texture2D* texture);
void Material_Bind(const Material* material);
void Material_Clear(Material* material);

// For PBR
void Material_SetAlbedo(Material* material, const Vec3& albedo);
void Material_SetEmission(Material* material, float emission);
void Material_SetMetallic(Material* material, float metallic);
void Material_SetRoughness(Material* material, float roughness);
void Material_SetUseNormalMap(Material* material, bool useNormalMap);
void Material_SetNormalMap(Material* material, const Texture2D* texture);
void Material_SetMetallicRoughnessMap(Material* material, const Texture2D* texture);
void Material_SetAlbedoMap(Material* material, const Texture2D* texture);
Vec3 Material_GetAlbedo(const Material* material);
float Material_GetEmission(const Material* material);
float Material_GetMetallic(const Material* material);
float Material_GetRoughness(const Material* material);
bool Material_GetUseNormalMap(const Material* material);