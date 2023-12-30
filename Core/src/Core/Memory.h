#pragma once

#include <stdint.h>

enum MemoryBlockTag
{
	MemoryBlockTag_Unknown = 0,
	MemoryBlockTag_String,
	MemoryBlockTag_Array,
	MemoryBlockTag_List,
	MemoryBlockTag_HashMap,
	MemoryBlockTag_Shader,
	MemoryBlockTag_Texture,
	MemoryBlockTag_Component,
	MemoryBlockTag_Buffer,
	MemoryBlockTag_CPPNew,
	MemoryBlockTag_Count
};

void* Memory_Allocate(uint32_t size, MemoryBlockTag tag);
void Memory_Copy(void* dest, const void* src, uint32_t size);
void Memory_Free(void* ptr, uint32_t size, MemoryBlockTag tag);

void Memory_CheckMemoryLeaks();

void* operator new(size_t size);
void operator delete(void* ptr, size_t size);