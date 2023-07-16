#pragma once
#include <stdint.h>

struct List
{
	void** Data = nullptr;
	uint32_t Index = 0;
	uint32_t Capacity = 50;
};

void List_Create(List* out);
void* List_Get(List* out, uint32_t index);
void List_Set(List* out, uint32_t index, void* data);

/// <summary>
/// Notice: This list stores pointers to the data, not the data itself.
/// Make sure the data is allocated on the heap.
/// Make sure the data is not freed while the list is still in use.
/// </summary>
void List_Add(List* out, void* data);

void List_RemoveAt(List* out, uint32_t index);
void List_Clear(List* out);
void List_Free(List* out);
uint32_t List_Size(List* out);