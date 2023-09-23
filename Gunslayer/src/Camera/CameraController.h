#pragma once
#include "Core.h"

void CameraController_OnCreate(Entity* entity, void* runtimeData);
void CameraController_OnUpdate(Entity* entity, float timeStep, void* runtimeData);
void CameraController_OnDestroy(Entity* entity, void* runtimeData);
void CameraController_OnCollision(Entity* entity, Entity* other, void* runtimeData);
void CameraController_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData);
void CameraController_OnEnable(Entity* entity, void* runtimeData);
void CameraController_OnDisable(Entity* entity, void* runtimeData);

Vec2 CameraController_GetForward();
Vec2 CameraController_GetRight();