#pragma once
#include <stdint.h>

// This is a simple list implementation
// Data is stored like this:
// [---Data---][---Data---][---Data---][---Data---][---Data---]

struct List
{
	uint8_t* Data = nullptr;
	uint32_t Length = 0;
	uint32_t Stride = 0;
	uint32_t Capacity = 50;
};

void List_Create(List& list, uint32_t stride, uint32_t capacity = 50);
const void* List_Get(const List& list, uint32_t index);
void List_Set(List& list, uint32_t index, void* data);
void* List_Add(List& list, void* data);
void List_RemoveAt(List& list, uint32_t index);
void List_Clear(List& list);
void List_Foreach(const List& list, void(*callback)(void* data));
void List_Free(List& list);
uint32_t List_Size(const List& list);
// Returns the pointer to the first element
void* List_GetData(const List& list);