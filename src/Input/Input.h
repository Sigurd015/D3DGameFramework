#pragma once
#include "KeyCode.h"

#define BIT(x) (1 << x)

enum Key
{
	ENTER,UPARROW,DOWNARROW,LEFTARROW,RIGHTARROW,BACK,
	P1_UP,P1_DOWN,P1_LEFT,P1_RIGHT,P1_BOMB,
	P2_UP,P2_DOWN,P2_LEFT,P2_RIGHT,P2_BOMB,
};

void Input_Process();
bool Input_SetKey(Key key, KeyCode code);
KeyCode Input_GetKey(Key key);
bool Input_IsKeyPressed(Key key);