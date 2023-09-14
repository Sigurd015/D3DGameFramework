#pragma once
#include "Input/Keycode.h"
#include "Input/MouseCode.h"

#include <stdint.h>

enum EventType
{
	None = 0,
	EventType_WindowClose, EventType_WindowResize,
	EventType_KeyPressed, EventType_KeyReleased, EventType_KeyTyped,
	EventType_MouseButtonPressed, EventType_MouseButtonReleased, EventType_MouseMoved, EventType_MouseScrolled
};

struct WndResizeEvnet
{
	uint32_t Width;
	uint32_t Height;
};

struct KeyPressedEvent
{
	KeyCode Keycode;
	bool IsRepeat;
};

struct KeyReleasedEvent
{
	KeyCode keycode;
};

struct KeyTypedEvent
{
	KeyCode keycode;
};

struct MouseButtonPressedEvent
{
	MouseCode Button;
};

struct MouseButtonReleasedEvent
{
	MouseCode Button;
};

struct MouseMovedEvent
{
	float x;
	float y;
};

struct MouseScrolledEvent
{
	float yOffset;
};

struct Event
{
	EventType Type;
	bool handled = false;
	void* Data = nullptr;
};

void Event_Dispatcher(EventType type, Event* event, bool(*Callback)(const Event* e));
Event* Event_Create(EventType type, void* data = nullptr);
void Event_Release(Event* event);