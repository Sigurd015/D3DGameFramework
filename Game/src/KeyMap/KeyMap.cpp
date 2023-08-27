#include "KeyMap.h"

struct KeyMapData
{
	KeyCode MoveForward = KeyCode::W;
	KeyCode MoveBackward = KeyCode::S;
	KeyCode MoveLeft = KeyCode::A;
	KeyCode MoveRight = KeyCode::D;
	KeyCode Shoot = KeyCode::Space;
};
static KeyMapData s_Data;

void KeyMap_Init(void)
{
	s_Data = {};
}

bool IsKeyValid(KeyCode key)
{
	return ((key >= KeyCode::A && key <= KeyCode::Z) || key == KeyCode::Space)
		&& key != s_Data.MoveForward
		&& key != s_Data.MoveBackward
		&& key != s_Data.MoveLeft
		&& key != s_Data.MoveRight
		&& key != s_Data.Shoot;
}

bool KeyMap_SetKey(KeyType type, KeyCode key)
{
	if (!IsKeyValid(key))
		return false;

	switch (type)
	{
	case MOVE_FORWARD:
		s_Data.MoveForward = key;
		break;
	case MOVE_LEFT:
		s_Data.MoveLeft = key;
		break;
	case MOVE_RIGHT:
		s_Data.MoveRight = key;
		break;
	case MOVE_BACKWARD:
		s_Data.MoveBackward = key;
		break;
	case SHOOT:
		s_Data.Shoot = key;
		break;
	}
	return true;
}

KeyCode KeyMap_GetKey(KeyType type)
{
	switch (type)
	{
	case MOVE_FORWARD:
		return s_Data.MoveForward;
	case MOVE_LEFT:
		return s_Data.MoveLeft;
	case MOVE_RIGHT:
		return s_Data.MoveRight;
	case MOVE_BACKWARD:
		return s_Data.MoveBackward;
	case SHOOT:
		return s_Data.Shoot;
	}
}

const WCHAR* GetKeyName(KeyCode key)
{

	switch (key)
	{
	case A:
		return L"A";
	case B:
		return L"B";
	case C:
		return L"C";
	case D:
		return L"D";
	case E:
		return L"E";
	case F:
		return L"F";
	case G:
		return L"G";
	case H:
		return L"H";
	case I:
		return L"I";
	case J:
		return L"J";
	case K:
		return L"K";
	case L:
		return L"L";
	case M:
		return L"M";
	case N:
		return L"N";
	case O:
		return L"O";
	case P:
		return L"P";
	case Q:
		return L"Q";
	case R:
		return L"R";
	case S:
		return L"S";
	case T:
		return L"T";
	case U:
		return L"U";
	case V:
		return L"V";
	case W:
		return L"W";
	case X:
		return L"X";
	case Y:
		return L"Y";
	case Z:
		return L"Z";
	case Space:
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
		return GetKeyName(s_Data.MoveForward);
	case MOVE_LEFT:
		return GetKeyName(s_Data.MoveLeft);
	case MOVE_RIGHT:
		return GetKeyName(s_Data.MoveRight);
	case MOVE_BACKWARD:
		return GetKeyName(s_Data.MoveBackward);
	case SHOOT:
		return GetKeyName(s_Data.Shoot);
	}
}