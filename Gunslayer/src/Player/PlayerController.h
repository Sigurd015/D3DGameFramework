#pragma once
#include "Core.h"

void PlayerController_OnCreate(Entity* entity, void* runtimeData);
void PlayerController_OnUpdate(Entity* entity, float timeStep, void* runtimeData);
void PlayerController_OnDestroy(Entity* entity, void* runtimeData);
void PlayerController_OnCollision(Entity* entity, Entity* other, void* runtimeData);
void PlayerController_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData);
void PlayerController_OnEnable(Entity* entity, void* runtimeData);
void PlayerController_OnDisable(Entity* entity, void* runtimeData);

float PlayerController_GetHpPercent();
void PlayerController_AddHp(float hp);