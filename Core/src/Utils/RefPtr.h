#pragma once
#include <cstdint>

struct RefPtr
{
	void* Ptr;
	uint32_t RefCount;
};

RefPtr* RefPtr_Create(uint32_t size, void* ptr);
void RefPtr_Reset(RefPtr* refPtr, uint32_t size, void* ptr);
RefPtr* RefPtr_AddRef(RefPtr* refPtr);
void* RefPtr_Get(RefPtr* refPtr);
void RefPtr_Release(RefPtr* refPtr, void(*ReleaseFunc)(void* prt));