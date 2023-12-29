#include "pch.h"
#include "List.h"

#define LIST_RESIZE_FACTOR 2

void List_Create(List& list, uint32_t stride, uint32_t capacity)
{
	list.Capacity = capacity;
	list.Stride = stride;
	list.Length = 0;
	list.Data = Memory_Allocate(list.Capacity * list.Stride, MemoryBlockTag_List);
}

const void* List_Get(const List& list, uint32_t index)
{
	if (index >= list.Length)
	{
		return nullptr;
	}
	return (void*)((uint8_t*)list.Data + (index * list.Stride));
}

void List_Set(List& list, uint32_t index, void* data)
{
	if (index >= list.Length)
	{
		return;
	}
	Memory_Copy((void*)((uint8_t*)list.Data + (index * list.Stride)), data, list.Stride);
}

void* List_Add(List& list, void* data)
{
	if (list.Length >= list.Capacity)
	{
		uint32_t oldCapacity = list.Capacity;
		void* oldData = list.Data;

		list.Capacity *= LIST_RESIZE_FACTOR;
		list.Data = Memory_Allocate(list.Capacity * list.Stride, MemoryBlockTag_List);
		Memory_Copy(list.Data, oldData, oldCapacity * list.Stride);
		Memory_Free(oldData, oldCapacity * list.Stride, MemoryBlockTag_List);
	}

	void* dest = (void*)((uint8_t*)list.Data + (list.Length * list.Stride));
	Memory_Copy(dest, data, list.Stride);
	list.Length++;
	return dest;
}

void List_RemoveAt(List& list, uint32_t index)
{
	if (index >= list.Length)
	{
		return;
	}

	for (uint32_t i = index; i < list.Length - 1; i++)
	{
		Memory_Copy((void*)((uint8_t*)list.Data + (i * list.Stride)),
			(void*)((uint8_t*)list.Data + ((i + 1) * list.Stride)), list.Stride);
	}

	list.Length--;
}

void List_Clear(List& list)
{
	list.Length = 0;
}

void List_Foreach(const List& list, void(*callback)(void* data))
{
	for (uint32_t i = 0; i < list.Length; i++)
	{
		callback((void*)((uint8_t*)list.Data + (i * list.Stride)));
	}
}

void List_Free(List& list)
{
	Memory_Free(list.Data, list.Capacity * list.Stride, MemoryBlockTag_List);
}

uint32_t List_Size(const List& list)
{
	return list.Length;
}