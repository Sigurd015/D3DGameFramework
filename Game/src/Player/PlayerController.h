#pragma once
#include "Core.h"

void PlayerController_OnCreate(Entity& entity);
void PlayerController_OnUpdate(Entity& entity, float timeStep);
void PlayerController_OnDestroy(Entity& entity);
void PlayerController_OnCollision(Entity& entity, Entity& other);
void PlayerController_OnRaycastHit(Entity& entity, Entity& other);

float PlayerController_GetHpPercent();