#pragma once
#include "Core.h"

void FieldTrigger_OnCreate(Entity& entity, void* runtimeData);
void FieldTrigger_OnUpdate(Entity& entity, float timeStep, void* runtimeData);
void FieldTrigger_OnDestroy(Entity& entity, void* runtimeData);
void FieldTrigger_OnCollision(Entity& entity, Entity& other, void* runtimeData);
void FieldTrigger_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData);
void FieldTrigger_OnEnable(Entity& entity, void* runtimeData);
void FieldTrigger_OnDisable(Entity& entity, void* runtimeData);