#pragma once
#include <stdint.h>

struct HashNode
{
	char* Key = nullptr;
	HashNode* Next = nullptr;
};

struct HashMap
{
	void* Table;
	bool IsPtrType;
	uint32_t ElementCount;
	uint32_t ElementSize;
};

void HashMap_Create(HashMap& hashMap, bool isPtrType = true, uint32_t elementSize = 0, uint32_t elementCount = 100);
void* HashMap_Find(const HashMap& hashMap, const char* key);
void HashMap_Set(HashMap& hashMap, const char* key, void* value);
void HashMap_Fill(HashMap& hashMap, void* value);

// Custom callback function for the value, whatever hashmap is ptr type or not
void HashMap_Foreach(const HashMap& hashMap, void(*callback)(void* value));
void HashMap_Free(HashMap& hashMap);