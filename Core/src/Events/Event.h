#pragma once
#include "Input/KeyCode.h"
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
	KeyCode Keycode;
	float padding;
};

struct KeyTypedEvent
{
	KeyCode Keycode;
	float padding;
};

struct MouseButtonPressedEvent
{
	MouseCode Button;
	float padding;
};

struct MouseButtonReleasedEvent
{
	MouseCode Button;
	float padding;
};

struct MouseMovedEvent
{
	float x;
	float y;
};

struct MouseScrolledEvent
{
	float yOffset;
	float padding;
};

struct EventContext
{
	union
	{
		WndResizeEvnet WndResize;
		KeyPressedEvent KeyPressed;
		KeyReleasedEvent KeyReleased;
		KeyTypedEvent KeyTyped;
		MouseButtonPressedEvent MouseButtonPressed;
		MouseButtonReleasedEvent MouseButtonReleased;
		MouseMovedEvent MouseMoved;
		MouseScrolledEvent MouseScrolled;
	};
};

struct Event
{
	EventType Type;
	bool Handled = false;
	EventContext Context;
};

void Event_Dispatcher(EventType type, Event& event, bool(*Callback)(Event e));
Event Event_Create(EventType type, const EventContext& context);