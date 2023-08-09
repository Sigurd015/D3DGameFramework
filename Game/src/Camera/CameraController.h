#pragma once
#include "Core.h"

void CameraController_OnCreate(Entity& entity);
void CameraController_OnUpdate(Entity& entity, float timeStep);
void CameraController_OnDestroy(Entity& entity);
void CameraController_OnCollision(Entity& entity, Entity& other);
void CameraController_OnRaycastHit(Entity& entity, Entity& other);