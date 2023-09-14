#include "pch.h"
#include "Event.h"

void Event_Dispatcher(EventType type, Event* event, bool(*Callback)(const Event* e))
{
	if (event->Type == type)
	{
		event->handled |= Callback(event);
	}
}

Event* Event_Create(EventType type, void* data)
{
	Event* e = (Event*)malloc(sizeof(Event));
	e->Data = nullptr;
	e->Type = type;
	e->handled = false;
	if (data)
	{
		switch (type)
		{
		case EventType_WindowResize:
		{
			e->Data = malloc(sizeof(WndResizeEvnet));
			memcpy(e->Data, data, sizeof(WndResizeEvnet));
			break;
		}
		case EventType_KeyPressed:
		{
			e->Data = malloc(sizeof(KeyPressedEvent));
			memcpy(e->Data, data, sizeof(KeyPressedEvent));
			break;
		}
		case EventType_KeyReleased:
		{
			e->Data = malloc(sizeof(KeyReleasedEvent));
			memcpy(e->Data, data, sizeof(KeyReleasedEvent));
			break;
		}
		case EventType_KeyTyped:
		{
			e->Data = malloc(sizeof(KeyTypedEvent));
			memcpy(e->Data, data, sizeof(KeyTypedEvent));
			break;
		}
		case EventType_MouseButtonPressed:
		{
			e->Data = malloc(sizeof(MouseButtonPressedEvent));
			memcpy(e->Data, data, sizeof(MouseButtonPressedEvent));
			break;
		}
		case EventType_MouseButtonReleased:
		{
			e->Data = malloc(sizeof(MouseButtonReleasedEvent));
			memcpy(e->Data, data, sizeof(MouseButtonReleasedEvent));
			break;
		}
		case EventType_MouseMoved:
		{
			e->Data = malloc(sizeof(MouseMovedEvent));
			memcpy(e->Data, data, sizeof(MouseMovedEvent));
			break;
		}
		case EventType_MouseScrolled:
		{
			e->Data = malloc(sizeof(MouseScrolledEvent));
			memcpy(e->Data, data, sizeof(MouseScrolledEvent));
			break;
		}
		}
	}
	return e;
}

void Event_Release(Event* event)
{
	if (event->Data)
		free(event->Data);
	free(event);
}