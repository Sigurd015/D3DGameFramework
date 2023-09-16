#pragma once
#include <stdint.h>

struct List
{
	void** Data = nullptr;
	uint32_t Index = 0;
	uint32_t Capacity = 50;
};

void List_Create(List& list, uint32_t capacity = 50);
const void* List_Get(const List& list, uint32_t index);
void List_Set(List& list, uint32_t index, void* data);
void List_Add(List& list, void* data, uint32_t size = 0);
void List_RemoveAt(List& list, uint32_t index);
void List_Clear(List& list, bool freeEachElement);
void List_Free(List& list, bool freeEachElement);
uint32_t List_Size(const List& list);