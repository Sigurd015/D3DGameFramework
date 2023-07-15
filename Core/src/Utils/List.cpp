#include "pch.h"
#include "List.h"

#include <corecrt_malloc.h>

void List_Create(List* out)
{
	out->Data = (void**)malloc(out->Capacity * sizeof(void*));
	if (out->Data == nullptr)
	{
		CORE_LOG_ERROR("List - Memory allocation failed");
		return;
	}

	out->Count = 0;
	out->Index = 0;
}

void* List_Get(List* out, uint32_t index)
{
	if (index >= out->Count)
	{
		return nullptr;
	}
	return out->Data[index];
}

void List_Set(List* out, uint32_t index, void* data)
{
	if (index >= out->Count)
	{
		return;
	}
	out->Data[index] = data;
}

void List_Add(List* out, void* data)
{
	if (out->Count + 1 > out->Capacity)
	{
		out->Capacity *= 2;
		out->Data = (void**)realloc(out->Data, out->Capacity * sizeof(void*));
		if (out->Data == nullptr)
		{
			CORE_LOG_ERROR("List - Memory allocation failed");
			return;
		}
	}

	out->Data[out->Index] = data;
	out->Index++;
	out->Count++;
}

void List_RemoveAt(List* out, uint32_t index)
{
	if (index >= out->Count)
	{
		return;
	}

	for (uint32_t i = index; i < out->Count - 1; i++)
	{
		out->Data[i] = out->Data[i + 1];
	}

	out->Index--;
	out->Count--;
}

void List_Clear(List* out)
{
	out->Count = 0;
	out->Index = 0;
}

void List_Free(List* out)
{
	for (size_t i = 0; i < out->Count; i++)
	{
		free(out->Data[i]);
	}
	free(out->Data);
}

uint32_t List_Size(List* out)
{
	return out->Count;
}