#pragma once
#include "Core.h"

enum KeyType
{
	MOVE_FORWARD, MOVE_LEFT, MOVE_RIGHT, MOVE_BACKWARD, SHOOT
};

void KeyMap_Init();
bool KeyMap_SetKey(KeyType type, KeyCode key);
KeyCode KeyMap_GetKey(KeyType type);
const WCHAR* KeyMap_GetKeyName(KeyType type);