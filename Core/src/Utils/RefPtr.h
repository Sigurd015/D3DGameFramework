#pragma once
#include <cstdint>

struct RefPtr
{
	void* Ptr;
	uint32_t Size = 0;
	uint32_t RefCount = 1;

	inline RefPtr* operator =(RefPtr* other)
	{
		RefCount--;
		other->RefCount++;
		return other;
	};
};

RefPtr* RefPtr_Create(uint32_t size, const void* ptr);
void* RefPtr_Get(const RefPtr* refPtr);
void RefPtr_Release(RefPtr* refPtr, void(*ReleaseFunc)(void* ptr));