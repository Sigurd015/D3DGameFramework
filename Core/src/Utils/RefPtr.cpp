#include "pch.h"
#include "RefPtr.h"

RefPtr* RefPtr_Create(uint32_t size, const void* ptr)
{
	RefPtr* refPtr = (RefPtr*)Memory_Allocate(sizeof(RefPtr), MemoryBlockTag_RefPtr);
	refPtr->RefCount = 0;
	refPtr->Size = size;
	refPtr->Ptr = Memory_Allocate(size, MemoryBlockTag_RefPtr);
	Memory_Copy(refPtr->Ptr, ptr, size);
	return refPtr;
}

void* RefPtr_Get(const RefPtr* refPtr)
{
	return refPtr->Ptr;
}

void RefPtr_Release(RefPtr* refPtr, void(*ReleaseFunc)(void* ptr))
{
	refPtr->RefCount--;
	if (refPtr->RefCount == 0)
	{
		ReleaseFunc(refPtr->Ptr);
		Memory_Free(refPtr->Ptr, refPtr->Size, MemoryBlockTag_RefPtr);
		Memory_Free(refPtr, sizeof(RefPtr), MemoryBlockTag_RefPtr);
	}
}