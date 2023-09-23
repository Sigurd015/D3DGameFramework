#pragma once
#include "Core.h"

enum ItemTriggerType
{
	MEDKIT,
};
struct ItemTriggerData
{
	ItemTriggerType Type;
	TransformComponent* PlayerTransform = nullptr;
	TransformComponent* Transform = nullptr;

	Rigidbody2DComponent* Rigidbody = nullptr;

	Vec2 Forward = { 0.0f, -1.0f };
};

void ItemTrigger_OnCreate(Entity* entity, void* runtimeData);
void ItemTrigger_OnUpdate(Entity* entity, float timeStep, void* runtimeData);
void ItemTrigger_OnDestroy(Entity* entity, void* runtimeData);
void ItemTrigger_OnCollision(Entity* entity, Entity* other, void* runtimeData);
void ItemTrigger_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData);
void ItemTrigger_OnEnable(Entity* entity, void* runtimeData);
void ItemTrigger_OnDisable(Entity* entity, void* runtimeData);