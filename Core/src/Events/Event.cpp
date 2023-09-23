#include "pch.h"
#include "Event.h"

void Event_Dispatcher(EventType type, Event& event, bool(*Callback)(Event e))
{
	if (event.Type == type)
	{
		event.Handled |= Callback(event);
	}
}

Event Event_Create(EventType type, const EventContext& context)
{
	Event event;
	event.Type = type;
	event.Context = context;
	return event;
}