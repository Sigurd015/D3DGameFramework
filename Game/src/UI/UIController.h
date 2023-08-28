#pragma once
#include "Core.h"

void UIController_OnCreate(Entity& entity, void* runtimeData);
void UIController_OnUpdate(Entity& entity, float timeStep, void* runtimeData);
void UIController_OnDestroy(Entity& entity, void* runtimeData);
void UIController_OnCollision(Entity& entity, Entity& other, void* runtimeData);
void UIController_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData);
void UIController_OnEnable(Entity& entity, void* runtimeData);
void UIController_OnDisable(Entity& entity, void* runtimeData);

void UIController_PlayShootAnimation();
void UIController_PlayHitIcon();
void UIController_PlayBloodEffect();