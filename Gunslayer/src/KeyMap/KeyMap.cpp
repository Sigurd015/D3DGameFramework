#include "KeyMap.h"

struct KeyMapData
{
	//InputDeviceType DeviceType = InputDeviceType_Controller;
	InputDeviceType DeviceType = InputDeviceType_Keyboard;

	KeyCode KB_MoveForward = KeyCode_W;
	KeyCode KB_MoveBackward = KeyCode_S;
	KeyCode KB_MoveLeft = KeyCode_A;
	KeyCode KB_MoveRight = KeyCode_D;
	KeyCode KB_RotateLeft = KeyCode_J;
	KeyCode KB_RotateRight = KeyCode_L;
	KeyCode KB_Shoot = KeyCode_Space;
};
static KeyMapData s_Data;

void KeyMap_Init()
{
	s_Data = {};
}

bool IsKeyValid(KeyCode key)
{
	return ((key >= KeyCode_A && key <= KeyCode_Z) || key == KeyCode_Space)
		&& key != s_Data.KB_MoveForward
		&& key != s_Data.KB_MoveBackward
		&& key != s_Data.KB_MoveLeft
		&& key != s_Data.KB_MoveRight
		&& key != s_Data.KB_Shoot;
}

bool KeyMap_SetKey(KeyType type, KeyCode key)
{
	if (!IsKeyValid(key))
		return false;

	switch (type)
	{
	case MOVE_FORWARD:
		s_Data.KB_MoveForward = key;
		break;
	case MOVE_LEFT:
		s_Data.KB_MoveLeft = key;
		break;
	case MOVE_RIGHT:
		s_Data.KB_MoveRight = key;
		break;
	case MOVE_BACKWARD:
		s_Data.KB_MoveBackward = key;
		break;
	case SHOOT:
		s_Data.KB_Shoot = key;
		break;
	}
	return true;
}

bool KeyMap_GetKey(KeyType type)
{
	switch (s_Data.DeviceType)
	{
	case InputDeviceType_Keyboard:
	{
		switch (type)
		{
		case MOVE_FORWARD:
			return Input_GetKey(s_Data.KB_MoveForward);
		case MOVE_LEFT:
			return Input_GetKey(s_Data.KB_MoveLeft);
		case MOVE_RIGHT:
			return Input_GetKey(s_Data.KB_MoveRight);
		case MOVE_BACKWARD:
			return Input_GetKey(s_Data.KB_MoveBackward);
		case ROTATE_LEFT:
			return Input_GetKey(s_Data.KB_RotateLeft);
		case ROTATE_RIGHT:
			return Input_GetKey(s_Data.KB_RotateRight);
		case SHOOT:
			return Input_GetKeyDown(s_Data.KB_Shoot);
		case MENU_UP:
			return Input_GetKeyDown(KeyCode_UpArrow);
		case MENU_DOWN:
			return Input_GetKeyDown(KeyCode_DownArrow);
		case MENU_LEFT:
			return Input_GetKeyDown(KeyCode_LeftArrow);
		case MENU_RIGHT:
			return Input_GetKeyDown(KeyCode_RightArrow);
		case MENU_SELECT:
			return Input_GetKeyDown(KeyCode_Enter);
		case MENU_BACK:
			return Input_GetKeyDown(KeyCode_Esc);
		}
	}
	case InputDeviceType_Controller:
	{
		switch (type)
		{
		case SHOOT:
			return Input_GetKeyDown(GamePadCode_RightTrigger);
		case MENU_UP:
			return Input_GetKeyDown(GamePadCode_DPadUp);
		case MENU_DOWN:
			return Input_GetKeyDown(GamePadCode_DPadDown);
		case MENU_LEFT:
			return Input_GetKeyDown(GamePadCode_DPadLeft);
		case MENU_RIGHT:
			return Input_GetKeyDown(GamePadCode_DPadRight);
		case MENU_SELECT:
			return Input_GetKeyDown(GamePadCode_A);
		case MENU_BACK:
			return Input_GetKeyDown(GamePadCode_B);
		}
	}
	}
}

Vec2 KeyMap_GetJoyStick(JoyStickType type)
{
	if (s_Data.DeviceType != InputDeviceType_Controller)
		return Vec2Zero;

	switch (type)
	{
	case JOY_MOVE:
		return Input_GetLeftStick();
	case JOY_ROTATE:
		return Input_GetRightStick();
	}
}

const WCHAR* GetKeyName(KeyCode key)
{
	switch (key)
	{
	case KeyCode_A:
		return L"A";
	case KeyCode_B:
		return L"B";
	case KeyCode_C:
		return L"C";
	case KeyCode_D:
		return L"D";
	case KeyCode_E:
		return L"E";
	case KeyCode_F:
		return L"F";
	case KeyCode_G:
		return L"G";
	case KeyCode_H:
		return L"H";
	case KeyCode_I:
		return L"I";
	case KeyCode_J:
		return L"J";
	case KeyCode_K:
		return L"K";
	case KeyCode_L:
		return L"L";
	case KeyCode_M:
		return L"M";
	case KeyCode_N:
		return L"N";
	case KeyCode_O:
		return L"O";
	case KeyCode_P:
		return L"P";
	case KeyCode_Q:
		return L"Q";
	case KeyCode_R:
		return L"R";
	case KeyCode_S:
		return L"S";
	case KeyCode_T:
		return L"T";
	case KeyCode_U:
		return L"U";
	case KeyCode_V:
		return L"V";
	case KeyCode_W:
		return L"W";
	case KeyCode_X:
		return L"X";
	case KeyCode_Y:
		return L"Y";
	case KeyCode_Z:
		return L"Z";
	case KeyCode_Space:
		return L"Space";
	default:
		return L"Unknown Key";
	}
}

const WCHAR* KeyMap_GetKeyName(KeyType type)
{
	switch (type)
	{
	case MOVE_FORWARD:
		return GetKeyName(s_Data.KB_MoveForward);
	case MOVE_LEFT:
		return GetKeyName(s_Data.KB_MoveLeft);
	case MOVE_RIGHT:
		return GetKeyName(s_Data.KB_MoveRight);
	case MOVE_BACKWARD:
		return GetKeyName(s_Data.KB_MoveBackward);
	case SHOOT:
		return GetKeyName(s_Data.KB_Shoot);
	}
}

void KeyMap_SetInputDeviceType(InputDeviceType type)
{
	s_Data.DeviceType = type;
}

InputDeviceType KeyMap_GetInputDeviceType()
{
	return s_Data.DeviceType;
}