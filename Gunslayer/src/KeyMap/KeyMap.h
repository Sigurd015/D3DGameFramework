#pragma once
#include "Core.h"

enum InputDeviceType
{
	InputDeviceType_Keyboard,
	InputDeviceType_Controller,
};

enum KeyType
{
	MOVE_FORWARD, MOVE_LEFT, MOVE_RIGHT, MOVE_BACKWARD, ROTATE_LEFT, ROTATE_RIGHT, SHOOT,
	MENU_UP, MENU_DOWN, MENU_LEFT, MENU_RIGHT, MENU_SELECT, MENU_BACK,
};

enum JoyStickType
{
	JOY_MOVE, JOY_ROTATE
};

void KeyMap_Init();
bool KeyMap_SetKey(KeyType type, KeyCode key);
bool KeyMap_GetKey(KeyType type);
Vec2 KeyMap_GetJoyStick(JoyStickType type);
const WCHAR* KeyMap_GetKeyName(KeyType type);
void KeyMap_SetInputDeviceType(InputDeviceType type);
InputDeviceType KeyMap_GetInputDeviceType();