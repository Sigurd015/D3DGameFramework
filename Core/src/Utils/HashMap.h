#pragma once
#include <stdint.h>

#define HashMapEnd nullptr

struct HashNode
{
	char* Key;
	void* Value = nullptr;
	struct HashNode* Next = nullptr;
};

struct HashMap
{
	HashNode** Table;
};

void HashMap_Create(HashMap& hashMap);
void HashMap_Set(HashMap& hashMap, const char* key, void* value, uint32_t size = 0);
HashNode* HashMap_Find(const HashMap& hashMap, const char* key);
HashNode* HashMap_Get(const HashMap& hashMap, uint32_t index);
uint32_t HashMap_GetTableSize();
void HashMap_Free(HashMap& hashMap, bool freeEachElement);