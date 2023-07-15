#pragma once

#include <stdint.h>
#include <Windows.h>
#include <d3d11.h>

enum ImageFormat
{
	NoneFormat = 0,
	R8,
	RGB8,
	RGBA8,
	RGBA32F
};

struct TextureSpecification
{
	uint32_t Width = 1;
	uint32_t Height = 1;
	ImageFormat Format = ImageFormat::RGBA8;
	bool GenerateMips = true;
};

struct Texture2D
{
	ID3D11Texture2D* Texture;
	ID3D11ShaderResourceView* SRV;
	ID3D11SamplerState* SamplerState;
};

void Texture2D_Create(Texture2D* out, const char* path);
void Texture2D_Create(Texture2D* out, const TextureSpecification* spec);
void Texture2D_Bind(Texture2D* out, uint32_t slot);