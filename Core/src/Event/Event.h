#pragma once
#include <stdint.h>

#include "Input/Keycode.h"
#include "Input/MouseCodes.h"

enum EventType
{
	None = 0,
	WindowClose, WindowResize,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

struct Event
{
	EventType Type;
	uint32_t WindowWidth;
	uint32_t WindowHeight;
	KeyCodes KeyCode;
	MouseCodes MouseCode;
	float MouseX, MouseY;
	float MouseScrollX, MouseScrollY;
};