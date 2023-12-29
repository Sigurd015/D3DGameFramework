#include "pch.h"
#include "String.h"

#include <string.h>

uint32_t String_Length(const char* str)
{
	return strlen(str);
}

char* String_Duplicate(const char* str)
{
	uint32_t len = String_Length(str);
	char* dup = (char*)Memory_Allocate(len + 1, MemoryBlockTag_String);
	Memory_Copy(dup, str, len);
	dup[len] = '\0';
	return dup;
}

void String_Free(char* str)
{
	Memory_Free(str, String_Length(str), MemoryBlockTag_String);
}

bool String_Contains(const char* str1, const char* str2)
{
	return strstr(str1, str2) != 0;
}

bool String_Compare(const char* str1, const char* str2)
{
	return strcmp(str1, str2) == 0;
}

void String_Format(char* dest, const char* format, ...)
{
	if (!dest)
		return;

	va_list args;
	va_start(args, format);
	char buffer[1024];
	int written = vsprintf(buffer, format, args);
	buffer[written] = '\0';
	Memory_Copy(dest, buffer, written + 1);
	va_end(args);
}
