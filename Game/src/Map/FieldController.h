#pragma once
#include "Core.h"

void FieldController_OnCreate(Entity* entity, void* runtimeData);
void FieldController_OnUpdate(Entity* entity, float timeStep, void* runtimeData);
void FieldController_OnDestroy(Entity* entity, void* runtimeData);
void FieldController_OnCollision(Entity* entity, Entity* other, void* runtimeData);
void FieldController_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData);
void FieldController_OnEnable(Entity* entity, void* runtimeData);
void FieldController_OnDisable(Entity* entity, void* runtimeData);

void FieldController_GenMap();
void FieldController_OnEnemyDead(const Vec3& pos);