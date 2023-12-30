#include "pch.h"
#include "TextureLoader.h"
#include "Renderer/Image.h"

#include "stb_image.h"

Buffer TextureLoader_ToBufferFromFile(const char* path, TextureSpecification& spec)
{
	Buffer imageBuffer;

	int width, height, channels;
	if (stbi_is_hdr(path))
	{
		// Notice: Look like stbi_set_flip_vertically_on_load is global state, so remember to reset it
		stbi_set_flip_vertically_on_load(0);
		imageBuffer.Data = (uint8_t*)stbi_loadf(path, &width, &height, &channels, STBI_rgb_alpha);
		imageBuffer.Size = width * height * 4 * sizeof(float);
		spec.Format = ImageFormat_RGBA32F;
		spec.GenerateMips = false;
	}
	else
	{
		stbi_set_flip_vertically_on_load(1);
		imageBuffer.Data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
		imageBuffer.Size = width * height * 4;
		spec.Format = ImageFormat_RGBA;
	}

	spec.Width = width;
	spec.Height = height;
	return imageBuffer;
}

Buffer TextureLoader_ToBufferFromMemory(Buffer buffer, TextureSpecification& spec)
{
	Buffer imageBuffer;

	int width, height, channels;
	if (stbi_is_hdr_from_memory((const stbi_uc*)buffer.Data, (int)buffer.Size))
	{
		stbi_set_flip_vertically_on_load(0);
		imageBuffer.Data = (uint8_t*)stbi_loadf_from_memory((const stbi_uc*)buffer.Data, (int)buffer.Size, &width, &height, &channels, STBI_rgb_alpha);
		imageBuffer.Size = width * height * 4 * sizeof(float);
		spec.Format = ImageFormat_RGBA32F;
		spec.GenerateMips = false;
	}
	else
	{
		stbi_set_flip_vertically_on_load(1);
		imageBuffer.Data = stbi_load_from_memory((const stbi_uc*)buffer.Data, (int)buffer.Size, &width, &height, &channels, STBI_rgb_alpha);
		imageBuffer.Size = width * height * 4;
		spec.Format = ImageFormat_RGBA;
	}

	spec.Width = width;
	spec.Height = height;
	return imageBuffer;
}

bool TextureLoader_TryLoad(Texture2D* texture2D, const char* path)
{
	TextureSpecification spec;
	spec.GenerateMips = true;
	Buffer buffer = TextureLoader_ToBufferFromFile(path, spec);

	if (!buffer.Data)
	{
		CORE_LOG_ERROR("TextureLoader_TryLoad: Failed to load texture from file: %s", path);
		return false;
	}

	Texture2D_Create(texture2D, spec, buffer);
	return true;
}