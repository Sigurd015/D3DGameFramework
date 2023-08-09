#pragma once
#include "Core.h"

void FieldTrigger_OnCreate(Entity& entity);
void FieldTrigger_OnUpdate(Entity& entity, float timeStep);
void FieldTrigger_OnDestroy(Entity& entity);
void FieldTrigger_OnCollision(Entity& entity, Entity& other);
void FieldTrigger_OnRaycastHit(Entity& entity, Entity& other);