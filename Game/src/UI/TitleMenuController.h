#pragma once
#include "Core.h"

void TitleMenuController_OnCreate(Entity& entity);
void TitleMenuController_OnUpdate(Entity& entity, float timeStep);
void TitleMenuController_OnDestroy(Entity& entity);
void TitleMenuController_OnCollision(Entity& entity, Entity& other);
void TitleMenuController_OnRaycastHit(Entity& entity, Entity& other);