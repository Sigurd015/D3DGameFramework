#pragma once
#include <stdint.h>

struct List
{
	void** Data = nullptr;
	uint32_t Index = 0;
	uint32_t Capacity = 50;
};

void List_Create(List& out, uint32_t capacity = 50);
const void* List_Get(const List& out, uint32_t index);
void List_Set(List& out, uint32_t index, void* data);
void List_Add(List& out, uint32_t size, void* data);
void List_Add(List& out, void* data);
void List_RemoveAt(List& out, uint32_t index);
void List_Clear(List& out, bool freeEachElement);
void List_Free(List& out, bool freeEachElement);
uint32_t List_Size(const List& out);