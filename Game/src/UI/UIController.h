#pragma once
#include "Core.h"

void UIController_OnCreate(Entity& entity);
void UIController_OnUpdate(Entity& entity, float timeStep);
void UIController_OnDestroy(Entity& entity);
void UIController_OnCollision(Entity& entity, Entity& other);