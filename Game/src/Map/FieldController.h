#pragma once
#include "Core.h"

void FieldController_OnCreate(Entity& entity);
void FieldController_OnUpdate(Entity& entity, float timeStep);
void FieldController_OnDestroy(Entity& entity);
void FieldController_OnCollision(Entity& entity, Entity& other);

void FieldController_GenMap();