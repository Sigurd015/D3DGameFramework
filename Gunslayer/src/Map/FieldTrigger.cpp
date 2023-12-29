#include "FieldTrigger.h"
#include "FieldController.h"

void FieldTrigger_OnCreate(Entity* entity, void* runtimeData)
{}
void FieldTrigger_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{}
void FieldTrigger_OnDestroy(Entity* entity, void* runtimeData)
{}
void FieldTrigger_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData)
{}
void FieldTrigger_OnEnable(Entity* entity, void* runtimeData)
{}
void FieldTrigger_OnDisable(Entity* entity, void* runtimeData)
{}

void FieldTrigger_OnCollision(Entity* entity, Entity* other, void* runtimeData)
{
	FieldTriggerData* data = (FieldTriggerData*)runtimeData;

	switch (data->Type)
	{
	case FieldTriggerType::GEN_MAP:
	{
		if (String_Compare(other->Tag.Name, "Player"))
		{
			FieldController_GenMap();
		}
		break;
	}
	}
}