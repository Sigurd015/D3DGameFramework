#include "pch.h"
#include "HashMap.h"

uint64_t GenHash(const char* key, uint32_t elementCount)
{
	static const uint64_t multiplier = 97;

	uint64_t hash = 0;
	for (size_t i = 0; key[i]; i++)
	{
		hash = hash * multiplier + key[i];
	}
	return hash % elementCount;
}

void SetValue(HashNode* node, void* value, uint32_t elementSize)
{
	void* dest = (uint8_t*)node + sizeof(HashNode);
	Memory_Copy(dest, value, elementSize);
}

void FreeNode(HashNode* node, uint32_t elementSize, bool freeNode)
{
	if (node->Key != nullptr)
	{
		String_Free(node->Key);
		node->Key = nullptr;
	}

	if (freeNode)
		Memory_Free(node, (sizeof(HashNode) + elementSize), MemoryBlockTag_HashMap);
}

void HashMap_Create(HashMap& hashMap, uint32_t elementSize, uint32_t elementCount)
{
	hashMap.ElementCount = elementCount;
	hashMap.ElementSize = elementSize;
	hashMap.Table = (uint8_t*)Memory_Allocate((sizeof(HashNode) + hashMap.ElementSize) * elementCount, MemoryBlockTag_HashMap);
	HashNode emptyNode = {};
	HashMap_Fill(hashMap, &emptyNode);
}

void HashMap_Set(HashMap& hashMap, const char* key, void* value)
{
	uint64_t index = GenHash(key, hashMap.ElementCount);
	HashNode* node = (HashNode*)(hashMap.Table + index * (sizeof(HashNode) + hashMap.ElementSize));
	if (node->Key == nullptr)
	{
		node->Key = String_Duplicate(key);
		SetValue(node, value, hashMap.ElementSize);
	}
	else
	{
		HashNode* newNode = (HashNode*)(uint8_t*)Memory_Allocate((sizeof(HashNode) + hashMap.ElementSize), MemoryBlockTag_HashMap);
		newNode->Key = String_Duplicate(key);
		SetValue(newNode, value, hashMap.ElementSize);
		newNode->Next = nullptr;

		while (node->Next != nullptr)
		{
			node = node->Next;
		}
		node->Next = newNode;
	}
}

void* HashMap_Find(const HashMap& hashMap, const char* key)
{
	uint64_t index = GenHash(key, hashMap.ElementCount);
	HashNode* node = (HashNode*)(hashMap.Table + index * (sizeof(HashNode) + hashMap.ElementSize));
	if (node->Key == nullptr)
	{
		return nullptr;
	}
	else
	{
		while (node != nullptr)
		{
			if (String_Compare(node->Key, key))
			{
				return (void*)((uint8_t*)node + sizeof(HashNode));
			}
			node = node->Next;
		}
	}
	return nullptr;
}

void HashMap_Fill(HashMap& hashMap, void* value)
{
	for (size_t i = 0; i < hashMap.ElementCount; i++)
	{
		HashNode* currentNode = (HashNode*)(hashMap.Table + i * (sizeof(HashNode) + hashMap.ElementSize));

		SetValue(currentNode, value, hashMap.ElementSize);
	}
}

void HashMap_Free(HashMap& hashMap)
{
	for (size_t i = 0; i < hashMap.ElementCount; i++)
	{
		HashNode* currentNode = (HashNode*)(hashMap.Table + i * (sizeof(HashNode) + hashMap.ElementSize));

		// Notice: Free the string inside the node, but not the node itself, because it's allocated with the table
		FreeNode(currentNode, hashMap.ElementSize, false);

		currentNode = currentNode->Next;

		while (currentNode != nullptr)
		{
			HashNode* temp = currentNode;
			currentNode = currentNode->Next;

			// Notice: Free all nodes in the linked list
			FreeNode(temp, hashMap.ElementSize, true);
		}
	}

	// Notice: Free the table
	Memory_Free(hashMap.Table, (sizeof(HashNode) + hashMap.ElementSize) * hashMap.ElementCount, MemoryBlockTag_HashMap);
}

void HashMap_Foreach(const HashMap& hashMap, void(*callback)(void* value))
{
	for (size_t i = 0; i < hashMap.ElementCount; i++)
	{
		HashNode* currentNode = (HashNode*)(hashMap.Table + i * (sizeof(HashNode) + hashMap.ElementSize));

		if (currentNode->Key == nullptr)
		{
			continue;
		}

		do {
			void* dest = (uint8_t*)currentNode + sizeof(HashNode);
			callback(dest);
			currentNode = currentNode->Next;
		} while (currentNode != nullptr);
	}
}