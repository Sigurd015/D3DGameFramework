#include "pch.h"
#include "HashMap.h"

#define TABLE_SIZE 100

void HashMap_Create(HashMap& hashMap)
{
	hashMap.Table = (HashNode**)malloc(TABLE_SIZE * sizeof(HashNode*));
	for (size_t i = 0; i < TABLE_SIZE; i++)
	{
		hashMap.Table[i] = nullptr;
	}
}

uint32_t GenHash(const char* key)
{
	uint32_t hash = 0;
	for (size_t i = 0; key[i]; i++)
	{
		hash += key[i];
	}
	return hash % TABLE_SIZE;
}

void HashMap_Set(HashMap& hashMap, const char* key, void* value, uint32_t size)
{
	uint32_t index = GenHash(key);
	HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
	newNode->Key = strdup(key);
	if (size != 0)
	{
		newNode->Value = malloc(size);
		memcpy(newNode->Value, value, size);
	}
	else
	{
		newNode->Value = value;
	}
	newNode->Next = nullptr;

	if (hashMap.Table[index] == nullptr)
	{
		hashMap.Table[index] = newNode;
	}
	else
	{
		HashNode* currentNode = hashMap.Table[index];
		while (currentNode->Next != nullptr)
		{
			currentNode = currentNode->Next;
		}
		currentNode->Next = newNode;
	}
}

HashNode* HashMap_Find(const HashMap& hashMap, const char* key)
{
	unsigned int index = GenHash(key);
	HashNode* currentNode = hashMap.Table[index];

	while (currentNode != nullptr)
	{
		if (strcmp(currentNode->Key, key) == 0)
		{
			return currentNode;
		}
		currentNode = currentNode->Next;
	}

	return nullptr;
}

void HashMap_Free(HashMap& hashMap, bool freeEachElement)
{
	for (size_t i = 0; i < TABLE_SIZE; i++)
	{
		HashNode* currentNode = hashMap.Table[i];
		if (currentNode == nullptr)
		{
			continue;
		}

		while (currentNode->Next != nullptr)
		{
			HashNode* nextNode = currentNode->Next;
			free(currentNode->Key);
			if (freeEachElement)
				free(currentNode->Value);
			free(currentNode);
			currentNode = nextNode;
		}
	}
	free(hashMap.Table);
}

uint32_t HashMap_GetTableSize()
{
	return TABLE_SIZE;
}

HashNode* HashMap_Get(const HashMap& hashMap, uint32_t index)
{
	CORE_ASSERT(index < TABLE_SIZE, "Index out of range");
	return hashMap.Table[index];
}

void* HashMap_Find(HashMap* hashMap, const char* key)
{
	return nullptr;
}
