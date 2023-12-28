#include "pch.h"
#include "Memory.h"

#include <corecrt_malloc.h>

struct MemoryStats
{
	uint32_t TotalAllocated = 0;
	uint32_t TotalAllocatedByTag[MemoryBlockTag_Count];
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

void Memory_CheckMemoryLeaks()
{
	if (s_MemoryStats.TotalAllocated)
	{
		CORE_LOG_ERROR("Memory_CheckMemoryLeaks: Memory leak detected");
		CORE_LOG_ERROR("Memory_CheckMemoryLeaks: Total allocated: %d", s_MemoryStats.TotalAllocated);
		for (size_t i = 0; i < MemoryBlockTag_Count; i++)
		{
			if (s_MemoryStats.TotalAllocatedByTag[i])
			{
				CORE_LOG_ERROR("Memory_CheckMemoryLeaks: Total allocated by tag %d: %d", i, s_MemoryStats.TotalAllocatedByTag[i]);
			}
		}
	}
}