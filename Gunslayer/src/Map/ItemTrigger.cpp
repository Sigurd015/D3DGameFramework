#include "ItemTrigger.h"
#include "Player/PlayerController.h"
#include "UI/UIController.h"

#define HEAL_AMOUNT 30.0f
#define ANGLE_OFFSET DirectX::XMConvertToRadians(90.0f)

void ItemTrigger_OnCreate(Entity* entity, void* runtimeData)
{
	ItemTriggerData* data = (ItemTriggerData*)runtimeData;

	Entity* Player = Scene_GetEntityByName(entity->Scene, "Player");
	CORE_ASSERT(Player, "Cannot find Player entity!");

	if (Player)
		data->PlayerTransform = (TransformComponent*)Entity_GetComponent(Player, ComponentType_Transform);
	CORE_ASSERT(data->PlayerTransform, "Entity does not have TransformComponent!");

	data->Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	CORE_ASSERT(data->Transform, "Entity does not have TransformComponent!");

	data->Rigidbody = (Rigidbody2DComponent*)Entity_GetComponent(entity, ComponentType_Rigidbody2D);
	CORE_ASSERT(data->Rigidbody, "Entity does not have Rigidbody2DComponent!");
}

void ItemTrigger_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{
	ItemTriggerData* data = (ItemTriggerData*)runtimeData;

	Vec2 playerPos = { data->PlayerTransform->Translation.x, data->PlayerTransform->Translation.y };
	Vec2 enemyPos = { data->Transform->Translation.x, data->Transform->Translation.y };
	Vec2 dir = Vec2Sub(playerPos, enemyPos);

	float angle = atan2f(dir.y, dir.x);
	data->Forward = { cosf(angle), sinf(angle) };

	angle += ANGLE_OFFSET;
	Rigidbody2DComponent_SetRotation(data->Rigidbody, angle);
}

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

			UIController_StartTimer();

			Scene_SetEntityEnabled(entity, false);
		}
		break;
	}
	}
}