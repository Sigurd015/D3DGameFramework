#include "pch.h"
#include "Memory.h"

#include <corecrt_malloc.h>

static const char* s_MemoryBlockStr[MemoryBlockTag_Count] =
{
	"Unknown",
	"String",
	"Array",
	"List",
	"HashMap",
	"Shader",
	"Texture",
	"Component",
	"Buffer",
	"CPPNew"
};

struct MemoryStats
{
	uint64_t TotalAllocated = 0;
	uint64_t TotalAllocatedByTag[MemoryBlockTag_Count];
};
static MemoryStats s_MemoryStats;

void* Memory_Allocate(uint32_t size, MemoryBlockTag tag)
{
	s_MemoryStats.TotalAllocated += size;
	s_MemoryStats.TotalAllocatedByTag[tag] += size;
	void* ptr = malloc(size);

	CORE_ASSERT(ptr, "Memory_Allocate: Memory allocation failed");
	memset(ptr, 0, size);
	return ptr;
}

void Memory_Copy(void* dest, const void* src, uint32_t size)
{
	memcpy(dest, src, size);
}

void Memory_Free(void* ptr, uint32_t size, MemoryBlockTag tag)
{
	free(ptr);
	s_MemoryStats.TotalAllocated -= size;
	s_MemoryStats.TotalAllocatedByTag[tag] -= size;
}

void* operator new(size_t size)
{
	void* ptr = Memory_Allocate(size, MemoryBlockTag_CPPNew);
	return ptr;
}

void operator delete(void* ptr, size_t size)
{
	Memory_Free(ptr, size, MemoryBlockTag_CPPNew);
}

// 1 byte = 8 bits
// 1 kilobyte = 1024 bytes
// 1 megabyte = 1024 kilobytes
// 1 gigabyte = 1024 megabytes

void Memory_CheckMemoryLeaks()
{
	if (s_MemoryStats.TotalAllocated)
	{
		CORE_LOG_ERROR("Memory_CheckMemoryLeaks: Memory leak detected");
		CORE_LOG_ERROR("----Total allocated: %d", s_MemoryStats.TotalAllocated);
		CORE_LOG_ERROR("----Total allocated : % d KB", s_MemoryStats.TotalAllocated / 1024);
		CORE_LOG_ERROR("----Total allocated : % d MB", s_MemoryStats.TotalAllocated / (1024 * 1024));
		CORE_LOG_ERROR("----Total allocated : % d GB", s_MemoryStats.TotalAllocated / (1024 * 1024 * 1024));
		for (size_t i = 0; i < MemoryBlockTag_Count; i++)
		{
			if (s_MemoryStats.TotalAllocatedByTag[i])
			{
				CORE_LOG_ERROR("--------Total allocated by tag %s: %d", s_MemoryBlockStr[i], s_MemoryStats.TotalAllocatedByTag[i]);
				CORE_LOG_ERROR("--------Total allocated by tag %s: %d KB", s_MemoryBlockStr[i], s_MemoryStats.TotalAllocatedByTag[i] / 1024);
				CORE_LOG_ERROR("--------Total allocated by tag %s: %d MB", s_MemoryBlockStr[i], s_MemoryStats.TotalAllocatedByTag[i] / (1024 * 1024));
				CORE_LOG_ERROR("--------Total allocated by tag %s: %d GB", s_MemoryBlockStr[i], s_MemoryStats.TotalAllocatedByTag[i] / (1024 * 1024 * 1024));			
			}
		}
	}
}