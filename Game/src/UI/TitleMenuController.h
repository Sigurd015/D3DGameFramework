#pragma once
#include "Core.h"

void TitleMenuController_OnCreate(Entity& entity, void* runtimeData);
void TitleMenuController_OnUpdate(Entity& entity, float timeStep, void* runtimeData);
void TitleMenuController_OnDestroy(Entity& entity, void* runtimeData);
void TitleMenuController_OnCollision(Entity& entity, Entity& other, void* runtimeData);
void TitleMenuController_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData);
void TitleMenuController_OnEnable(Entity& entity, void* runtimeData);
void TitleMenuController_OnDisable(Entity& entity, void* runtimeData);