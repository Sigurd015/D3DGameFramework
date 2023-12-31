#pragma once
#include <stdint.h>

// This is a simple hashmap implementation
// Data is stored like this:
// [HashNode][---Data---][HashNode][---Data---][HashNode][---Data---]
// If hash collision occurs, the data is stored like linked list

struct HashNode
{
	char* Key = nullptr;
	HashNode* Next = nullptr;
};

struct HashMap
{
	uint8_t* Table;
	uint32_t ElementCount;
	uint32_t ElementSize;
};

void HashMap_Create(HashMap& hashMap, uint32_t elementSize = 0, uint32_t elementCount = 100);
void* HashMap_Find(const HashMap& hashMap, const char* key);
void HashMap_Set(HashMap& hashMap, const char* key, void* value);
void HashMap_Fill(HashMap& hashMap, void* value);
void HashMap_Foreach(const HashMap& hashMap, void(*callback)(void* value));
void HashMap_Free(HashMap& hashMap);