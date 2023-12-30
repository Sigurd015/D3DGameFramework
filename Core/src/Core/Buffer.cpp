#include "pch.h"
#include "Buffer.h"

void Buffer_Create(Buffer& buffer, uint64_t size, const void* data, bool allocate)
{
	if (allocate)
	{
		Buffer_Copy(buffer, { (uint8_t*)data, size });
	}
	else
	{
		buffer.Data = (uint8_t*)data;
		buffer.Size = size;
	}
}

void Buffer_Release(Buffer& buffer)
{
	Memory_Free(buffer.Data, buffer.Size, MemoryBlockTag_Buffer);
	buffer.Data = nullptr;
	buffer.Size = 0;
}

void Buffer_Copy(Buffer& dest, const Buffer& src)
{
	dest.Data = (uint8_t*)Memory_Allocate(src.Size, MemoryBlockTag_Buffer);
	dest.Size = src.Size;

	Memory_Copy(dest.Data, src.Data, src.Size);
}