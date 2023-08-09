#pragma once
#include "Core.h"

void EnemyController_OnCreate(Entity& entity);
void EnemyController_OnUpdate(Entity& entity, float timeStep);
void EnemyController_OnDestroy(Entity& entity);
void EnemyController_OnCollision(Entity& entity, Entity& other);
void EnemyController_OnRaycastHit(Entity& entity, Entity& other);