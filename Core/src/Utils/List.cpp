#include "pch.h"
#include "List.h"

#include <corecrt_malloc.h>

void List_Create(List& list, uint32_t capacity)
{
	list.Capacity = capacity;
	list.Data = (void**)malloc(list.Capacity * sizeof(void*));

	CORE_ASSERT(list.Data != nullptr, "List_Create: Memory allocation failed");

	list.Index = 0;
}

const void* List_Get(const List& list, uint32_t index)
{
	if (index >= list.Index)
	{
		return nullptr;
	}
	return list.Data[index];
}

void List_Set(List& list, uint32_t index, void* data)
{
	if (index >= list.Index)
	{
		return;
	}
	list.Data[index] = data;
}

void List_Add(List& list, void* data, uint32_t size)
{
	if (list.Index + 1 > list.Capacity)
	{
		list.Capacity *= 2;
		list.Data = (void**)realloc(list.Data, list.Capacity * sizeof(void*));

		CORE_ASSERT(list.Data != nullptr, "List_Add: Memory allocation failed");
	}

	if (size != 0)
	{
		list.Data[list.Index] = (void*)malloc(size);
		memcpy(list.Data[list.Index], data, size);
	}
	else
	{
		list.Data[list.Index] = data;
	}
	list.Index++;
}

void List_RemoveAt(List& list, uint32_t index)
{
	if (index >= list.Index)
	{
		return;
	}

	for (uint32_t i = index; i < list.Index - 1; i++)
	{
		list.Data[i] = list.Data[i + 1];
	}

	list.Index--;
}

void List_Clear(List& list, bool freeEachElement)
{
	if (freeEachElement)
	{
		for (size_t i = 0; i < list.Index; i++)
		{
			if (list.Data[i] != nullptr)
				free(list.Data[i]);
		}
	}

	list.Index = 0;
}

void List_Free(List& list, bool freeEachElement)
{
	if (freeEachElement)
	{
		for (size_t i = 0; i < list.Index; i++)
		{
			if (list.Data[i] != nullptr)
				free(list.Data[i]);
		}
	}

	free(list.Data);
}

uint32_t List_Size(const List& list)
{
	return list.Index;
}