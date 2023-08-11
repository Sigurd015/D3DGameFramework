#include "pch.h"
#include "List.h"

#include <corecrt_malloc.h>

void List_Create(List& out, uint32_t capacity)
{
	out.Capacity = capacity;
	out.Data = (void**)malloc(out.Capacity * sizeof(void*));

	CORE_ASSERT(out.Data != nullptr, "List_Create: Memory allocation failed");

	out.Index = 0;
}

const void* List_Get(const List& out, uint32_t index)
{
	if (index >= out.Index)
	{
		return nullptr;
	}
	return out.Data[index];
}

void List_Set(List& out, uint32_t index, void* data)
{
	if (index >= out.Index)
	{
		return;
	}
	out.Data[index] = data;
}

void List_Add(List& out, uint32_t size, void* data)
{
	if (out.Index + 1 > out.Capacity)
	{
		out.Capacity *= 2;
		out.Data = (void**)realloc(out.Data, out.Capacity * sizeof(void*));

		CORE_ASSERT(out.Data != nullptr, "List_Add: Memory allocation failed");
	}

	out.Data[out.Index] = (void*)malloc(size);
	memcpy(out.Data[out.Index], data, size);
	out.Index++;
}

void List_Add(List& out, void* data)
{
	if (out.Index + 1 > out.Capacity)
	{
		out.Capacity *= 2;
		out.Data = (void**)realloc(out.Data, out.Capacity * sizeof(void*));

		CORE_ASSERT(out.Data != nullptr, "List_Add: Memory allocation failed");
	}

	out.Data[out.Index] = data;
	out.Index++;
}

void List_RemoveAt(List& out, uint32_t index)
{
	if (index >= out.Index)
	{
		return;
	}

	for (uint32_t i = index; i < out.Index - 1; i++)
	{
		out.Data[i] = out.Data[i + 1];
	}

	out.Index--;
}

void List_Clear(List& out, bool freeEachElement)
{
	if (freeEachElement)
	{
		for (size_t i = 0; i < out.Index; i++)
		{
			if (out.Data[i] != nullptr)
				free(out.Data[i]);
		}
	}

	out.Index = 0;
}

void List_Free(List& out, bool freeEachElement)
{
	if (freeEachElement)
	{
		for (size_t i = 0; i < out.Index; i++)
		{
			if (out.Data[i] != nullptr)
				free(out.Data[i]);
		}
	}

	free(out.Data);
}

uint32_t List_Size(const List& out)
{
	return out.Index;
}