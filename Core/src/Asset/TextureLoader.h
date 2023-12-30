#pragma once
#include "Core/Buffer.h"
#include "Renderer/Texture.h"

Buffer TextureLoader_ToBufferFromFile(const char* path, TextureSpecification& spec);
Buffer TextureLoader_ToBufferFromMemory(Buffer buffer, TextureSpecification& spec);

bool TextureLoader_TryLoad(Texture2D* texture2D, const char* path);