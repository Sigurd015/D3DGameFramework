#pragma once

#include <stdint.h>

uint32_t String_Length(const char* str);
char* String_Duplicate(const char* str);
void String_Free(char* str);
bool String_Contains(const char* str1, const char* str2);
bool String_Compare(const char* str1, const char* str2);
void String_Format(char* dest, const char* format, ...);