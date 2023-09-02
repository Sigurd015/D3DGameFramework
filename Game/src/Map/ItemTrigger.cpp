#include "ItemTrigger.h"
#include "Player/PlayerController.h"
#include "UI/UIController.h"

#define HEAL_AMOUNT 30.0f

void ItemTrigger_OnCreate(Entity* entity, void* runtimeData)
{}
void ItemTrigger_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{}
void ItemTrigger_OnDestroy(Entity* entity, void* runtimeData)
{}
void ItemTrigger_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData)
{}
void ItemTrigger_OnEnable(Entity* entity, void* runtimeData)
{}
void ItemTrigger_OnDisable(Entity* entity, void* runtimeData)
{}

void ItemTrigger_OnCollision(Entity* entity, Entity* other, void* runtimeData)
{
	ItemTriggerData* data = (ItemTriggerData*)runtimeData;

	switch (data->Type)
	{
	case ItemTriggerType::MEDKIT:
	{
		if (!strcmp(other->Tag.Name, "Player"))
		{
			PlayerController_AddHp(HEAL_AMOUNT);

			static bool firstEnter = true;
			if (firstEnter)
			{
				UIController_StartTimer();
				firstEnter = false;
			}

			Scene_SetEntityEnabled(entity, false);
		}
		break;
	}
	}
}