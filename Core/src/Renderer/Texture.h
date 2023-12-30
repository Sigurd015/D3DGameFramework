#pragma once
#include "Shader.h"
#include "Image.h"

#include <stdint.h>
#include <Windows.h>
#include <d3d11.h>

struct TextureSpecification
{
	uint32_t Width = 1;
	uint32_t Height = 1;
	ImageFormat Format = ImageFormat_None;
	bool GenerateMips = true;
};

struct Texture2D
{
	TextureSpecification Spec;

	Image2D Image;
};

void Texture2D_Create(Texture2D* texture2D, const TextureSpecification& spec, Buffer data);
void Texture2D_Bind(const Texture2D* texture2D, const ShaderResourceDeclaration* decl);
bool Texture2D_IsSame(const Texture2D* texture2D, const Texture2D* other);
void Texture2D_Release(Texture2D* texture2D);
uint32_t Texture2D_GetWidth(const Texture2D* texture2D);
uint32_t Texture2D_GetHeight(const Texture2D* texture2D);

struct TextureCube
{
	TextureSpecification Spec;

	ID3D11Texture2D* Texture = nullptr;
	ID3D11ShaderResourceView* TextureView = nullptr;
	DXGI_FORMAT DataFormat;

	bool NeedRelease = false;
};

void TextureCube_Create(TextureCube* textureCube, const TextureSpecification& spec);
void TextureCube_Bind(const TextureCube* textureCube, const ShaderResourceDeclaration* decl);
void TextureCube_Release(TextureCube* textureCube);
uint32_t TextureCube_GetWidth(const TextureCube* textureCube);
uint32_t TextureCube_GetHeight(const TextureCube* textureCube);