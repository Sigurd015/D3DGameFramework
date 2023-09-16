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
	ImageFormat Format = ImageFormat::NoneFormat;
};

struct Texture2D
{
	TextureSpecification Spec;

	ID3D11Texture2D* Texture;
	ID3D11ShaderResourceView* TextureView;
	DXGI_FORMAT DataFormat;
};

void Texture2D_Create(Texture2D* texture2D, const char* path);
void Texture2D_Create(Texture2D* texture2D, const TextureSpecification& spec);
void Texture2D_SetData(Texture2D* texture2D, void* data, uint32_t size);
void Texture2D_Bind(const Texture2D* texture2D, uint32_t slot);
bool Texture2D_IsSame(const Texture2D* texture2D, const Texture2D* other);
void Texture2D_Release(Texture2D* texture2D);
uint32_t Texture2D_GetWidth(const Texture2D* texture2D);
uint32_t Texture2D_GetHeight(const Texture2D* texture2D);