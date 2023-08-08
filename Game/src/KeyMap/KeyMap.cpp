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

void KeyMap_SetKey(KeyType type, KeyCode key)
{
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