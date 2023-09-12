#include "Input.h"

#include <conio.h>
#include <stdio.h>

struct KeyMap
{
	//メニューとかのシステムで使ています
	KeyCode Enter = KeyCode::Enter;
	KeyCode Up = KeyCode::UpArrow;
	KeyCode Down = KeyCode::DownArrow;
	KeyCode Left = KeyCode::LeftArrow;
	KeyCode Right = KeyCode::RightArrow;
	KeyCode Back = KeyCode::EscapeKey;

	//プレイヤーコントロール
	KeyCode P1MoveUp = KeyCode::W;
	KeyCode P1MoveDown = KeyCode::S;
	KeyCode P1MoveLeft = KeyCode::A;
	KeyCode P1MoveRight = KeyCode::D;
	KeyCode P1Bomb = KeyCode::F;

	KeyCode P2MoveUp = KeyCode::I;
	KeyCode P2MoveDown = KeyCode::K;
	KeyCode P2MoveLeft = KeyCode::J;
	KeyCode P2MoveRight = KeyCode::L;
	KeyCode P2Bomb = KeyCode::H;
};

enum KeyStateFlag
{
	NONE = 0,

	//メニューとかのシステムで使ています
	IS_ENTER_PRESSED = BIT(0),
	IS_UP_PRESSED = BIT(1),
	IS_DOWN_PRESSED = BIT(2),
	IS_LEFT_PRESSED = BIT(3),
	IS_RIGHT_PRESSED = BIT(4),
	IS_BACK_PRESSED = BIT(5),

	//プレイヤーコントロール
	IS_P1_MOVEUP_PRESSED = BIT(6),
	IS_P1_MOVEDOWN_PRESSED = BIT(7),
	IS_P1_MOVELEFT_PRESSED = BIT(8),
	IS_P1_MOVERIGHT_PRESSED = BIT(9),
	IS_P1_BOMB_PRESSED = BIT(10),

	IS_P2_MOVEUP_PRESSED = BIT(11),
	IS_P2_MOVEDOWN_PRESSED = BIT(12),
	IS_P2_MOVELEFT_PRESSED = BIT(13),
	IS_P2_MOVERIGHT_PRESSED = BIT(14),
	IS_P2_BOMB_PRESSED = BIT(15),
};

struct InputData
{
	KeyMap Key;
	KeyStateFlag KeyState;
};

InputData s_Data;

void Input_Process()
{
	s_Data.KeyState = KeyStateFlag::NONE;

	if (!_kbhit())
		return;

	int key;
	rewind(stdin);
	key = _getch();

	if (key == 0 || key == 0xe0)
		key = _getch();

	if (key == s_Data.Key.Back)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_BACK_PRESSED);

	if (key == s_Data.Key.Enter)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_ENTER_PRESSED);

	if (key == s_Data.Key.Up)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_UP_PRESSED);

	if (key == s_Data.Key.Down)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_DOWN_PRESSED);

	if (key == s_Data.Key.Left)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_LEFT_PRESSED);

	if (key == s_Data.Key.Right)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_RIGHT_PRESSED);

	if (key == s_Data.Key.P1MoveUp)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_P1_MOVEUP_PRESSED);

	if (key == s_Data.Key.P1MoveDown)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_P1_MOVEDOWN_PRESSED);

	if (key == s_Data.Key.P1MoveLeft)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_P1_MOVELEFT_PRESSED);

	if (key == s_Data.Key.P1MoveRight)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_P1_MOVERIGHT_PRESSED);

	if (key == s_Data.Key.P1Bomb)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_P1_BOMB_PRESSED);

	if (key == s_Data.Key.P2MoveUp)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_P2_MOVEUP_PRESSED);

	if (key == s_Data.Key.P2MoveDown)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_P2_MOVEDOWN_PRESSED);

	if (key == s_Data.Key.P2MoveLeft)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_P2_MOVELEFT_PRESSED);

	if (key == s_Data.Key.P2MoveRight)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_P2_MOVERIGHT_PRESSED);

	if (key == s_Data.Key.P2Bomb)
		s_Data.KeyState = (KeyStateFlag)(s_Data.KeyState | IS_P2_BOMB_PRESSED);
}

bool IsKeyCanSet(KeyCode code)
{
	return (code >= KeyCode::A) && (code <= KeyCode::Z)
		&& code != s_Data.Key.P1MoveLeft
		&& code != s_Data.Key.P1MoveRight
		&& code != s_Data.Key.P1MoveUp
		&& code != s_Data.Key.P1MoveDown
		&& code != s_Data.Key.P1Bomb
		&& code != s_Data.Key.P2MoveLeft
		&& code != s_Data.Key.P2MoveRight
		&& code != s_Data.Key.P2MoveUp
		&& code != s_Data.Key.P2MoveDown
		&& code != s_Data.Key.P2Bomb;
}

bool Input_SetKey(Key key, KeyCode code)
{
	if (IsKeyCanSet(code))
	{
		switch (key)
		{
		case ENTER: s_Data.Key.Enter = code; break;
		case UPARROW: s_Data.Key.Up = code; break;
		case DOWNARROW: s_Data.Key.Down = code; break;
		case LEFTARROW: s_Data.Key.Left = code; break;
		case RIGHTARROW: s_Data.Key.Right = code; break;
		case BACK: s_Data.Key.Back = code; break;
		case P1_UP: s_Data.Key.P1MoveUp = code; break;
		case P1_DOWN: s_Data.Key.P1MoveDown = code; break;
		case P1_LEFT: s_Data.Key.P1MoveLeft = code; break;
		case P1_RIGHT: s_Data.Key.P1MoveRight = code; break;
		case P1_BOMB: s_Data.Key.P1Bomb = code; break;
		case P2_UP: s_Data.Key.P2MoveUp = code; break;
		case P2_DOWN: s_Data.Key.P2MoveDown = code; break;
		case P2_LEFT: s_Data.Key.P2MoveLeft = code; break;
		case P2_RIGHT: s_Data.Key.P2MoveRight = code; break;
		case P2_BOMB: s_Data.Key.P2Bomb = code; break;
		}
		return true;
	}

	return false;
}

KeyCode Input_GetKey(Key key)
{
	switch (key)
	{
	case ENTER: return s_Data.Key.Enter;
	case UPARROW: return s_Data.Key.Up;
	case DOWNARROW: return s_Data.Key.Down;
	case LEFTARROW: return s_Data.Key.Left;
	case RIGHTARROW: return s_Data.Key.Right;
	case BACK: return s_Data.Key.Back;
	case P1_UP: return s_Data.Key.P1MoveUp;
	case P1_DOWN: return s_Data.Key.P1MoveDown;
	case P1_LEFT: return s_Data.Key.P1MoveLeft;
	case P1_RIGHT: return s_Data.Key.P1MoveRight;
	case P1_BOMB: return s_Data.Key.P1Bomb;
	case P2_UP: return s_Data.Key.P2MoveUp;
	case P2_DOWN: return s_Data.Key.P2MoveDown;
	case P2_LEFT: return s_Data.Key.P2MoveLeft;
	case P2_RIGHT: return s_Data.Key.P2MoveRight;
	case P2_BOMB: return s_Data.Key.P2Bomb;
	}
}

bool Input_IsKeyPressed(Key key)
{
	switch (key)
	{
	case ENTER: return s_Data.KeyState & IS_ENTER_PRESSED;
	case UPARROW: return s_Data.KeyState & IS_UP_PRESSED;
	case DOWNARROW: return s_Data.KeyState & IS_DOWN_PRESSED;
	case LEFTARROW: return s_Data.KeyState & IS_LEFT_PRESSED;
	case RIGHTARROW: return s_Data.KeyState & IS_RIGHT_PRESSED;
	case BACK: return s_Data.KeyState & IS_BACK_PRESSED;
	case P1_UP: return s_Data.KeyState & IS_P1_MOVEUP_PRESSED;
	case P1_DOWN: return s_Data.KeyState & IS_P1_MOVEDOWN_PRESSED;
	case P1_LEFT: return s_Data.KeyState & IS_P1_MOVELEFT_PRESSED;
	case P1_RIGHT: return s_Data.KeyState & IS_P1_MOVERIGHT_PRESSED;
	case P1_BOMB: return s_Data.KeyState & IS_P1_BOMB_PRESSED;
	case P2_UP: return s_Data.KeyState & IS_P2_MOVEUP_PRESSED;
	case P2_DOWN: return s_Data.KeyState & IS_P2_MOVEDOWN_PRESSED;
	case P2_LEFT: return s_Data.KeyState & IS_P2_MOVELEFT_PRESSED;
	case P2_RIGHT: return s_Data.KeyState & IS_P2_MOVERIGHT_PRESSED;
	case P2_BOMB: return s_Data.KeyState & IS_P2_BOMB_PRESSED;
	}
	return false;
}
