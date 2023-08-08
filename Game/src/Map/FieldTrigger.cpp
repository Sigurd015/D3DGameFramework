#include "FieldTrigger.h"
#include "FieldController.h"

void FieldTrigger_OnCreate(Entity& entity)
{}

void FieldTrigger_OnUpdate(Entity& entity, float timeStep)
{}

void FieldTrigger_OnDestroy(Entity& entity)
{}

void FieldTrigger_OnCollision(Entity& entity, Entity& other)
{
	if (!strcmp(other.Tag.Name, "Player"))
	{
		FieldController_GenMap();
	}
}