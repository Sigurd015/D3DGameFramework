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

	// Notice: When the texture is created, never use this data again!(it may be deleted)
	void* Data = nullptr;
	uint32_t DataSize = 0;
};

struct Texture2D
{
	TextureSpecification Spec;

	ID3D11Texture2D* Texture = nullptr;
	ID3D11ShaderResourceView* TextureView = nullptr;
	DXGI_FORMAT DataFormat;

	bool NeedRelease = false;
};

void* Texture2D_Create(const char* path);
void Texture2D_Create(Texture2D* texture2D, const TextureSpecification& spec);
void Texture2D_Bind(const Texture2D* texture2D, uint32_t slot);
bool Texture2D_IsSame(const Texture2D* texture2D, const Texture2D* other);
void Texture2D_Release(Texture2D* texture2D);
uint32_t Texture2D_GetWidth(const Texture2D* texture2D);
uint32_t Texture2D_GetHeight(const Texture2D* texture2D);