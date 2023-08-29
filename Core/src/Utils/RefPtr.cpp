#include "pch.h"
#include "RefPtr.h"

#ifndef CORE_DIST
static uint32_t RefPtrCount = 1;
#endif 

RefPtr* RefPtr_Create(uint32_t size, void* ptr)
{
	RefPtr* refPtr = (RefPtr*)malloc(sizeof(RefPtr));
	refPtr->RefCount = 0;
	refPtr->Ptr = (void*)malloc(size);
	memcpy(refPtr->Ptr, ptr, size);
	return refPtr;
}

RefPtr* RefPtr_Create(void* ptr)
{
	RefPtr* refPtr = (RefPtr*)malloc(sizeof(RefPtr));
	refPtr->Ptr = ptr;
	return refPtr;
}

void RefPtr_Reset(RefPtr* refPtr, uint32_t size, void* ptr)
{
	refPtr->Ptr = (void*)malloc(size);
	memcpy(refPtr->Ptr, ptr, size);
}

void RefPtr_Reset(RefPtr* refPtr, void* ptr)
{
	refPtr->Ptr = ptr;
}

RefPtr* RefPtr_AddRef(RefPtr* refPtr)
{
	refPtr->RefCount++;
	return refPtr;
}

void RefPtr_DecRef(RefPtr* refPtr)
{
	refPtr->RefCount--;
}

void* RefPtr_Get(RefPtr* refPtr)
{
	return refPtr->Ptr;
}

void RefPtr_Release(RefPtr* refPtr, void(*ReleaseFunc)(void* prt))
{
	refPtr->RefCount--;
	if (refPtr->RefCount == 0)
	{
		ReleaseFunc(refPtr->Ptr);

		#ifndef CORE_DIST
		CORE_LOG_INFO("Ref Ptr Count: ");
		CORE_LOG_INFO(ToString(RefPtrCount++));
		#endif 

		free(refPtr->Ptr);
		free(refPtr);
	}
}