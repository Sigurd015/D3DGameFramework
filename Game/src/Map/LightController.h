#pragma once
#include "Core.h"

enum LightType
{
	RED,GREEN
};

struct LightData
{
	LightType Type;
	SpriteRendererComponent* SpriteRenderer = nullptr;

	Texture2D* SpriteSheet = nullptr;
	SpriteAnimator SpriteAnimator;
	SpriteTimer SpriteTimer;
};

void LightController_OnCreate(Entity& entity, void* runtimeData);
void LightController_OnUpdate(Entity& entity, float timeStep, void* runtimeData);
void LightController_OnDestroy(Entity& entity, void* runtimeData);
void LightController_OnCollision(Entity& entity, Entity& other, void* runtimeData);
void LightController_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData);
void LightController_OnEnable(Entity& entity, void* runtimeData);
void LightController_OnDisable(Entity& entity, void* runtimeData);