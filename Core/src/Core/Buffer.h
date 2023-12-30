#pragma once

#include <stdint.h>

struct Buffer
{
	uint8_t* Data = nullptr;
	uint64_t Size = 0;
};

void Buffer_Create(Buffer& buffer, uint64_t size, const void* data, bool allocate = false);
void Buffer_Release(Buffer& buffer);
void Buffer_Copy(Buffer& dest, const Buffer& src);