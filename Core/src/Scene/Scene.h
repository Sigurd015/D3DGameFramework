#pragma once
#include "Entity.h"
#include "Physics/PhysicsWorld.h"

struct Scene
{
	List Entities;

	uint32_t ViewportWidth;
	uint32_t ViewportHeight;

	PhysicsWorld2D PhysicsWorld;
};

void Scene_Create(Scene& out);
void Scene_Ininialize(Scene& out);
void Scene_Destroy(Scene& out);
void Scene_AddEntity(Scene& out, Entity& entity);
Entity* Scene_GetEntityByName(const Scene* out, const char* name);
Entity* Scene_GetPrimaryCamera(const Scene& out);
void* Scene_GetListener(const Scene* out);
void Scene_SetEntityEnabled(Entity* entity, bool enabled);
void Scene_OnUpdate(Scene& out, float timeStep);
void Scene_OnViewportResize(Scene& out, uint32_t width, uint32_t height);
bool Scene_Raycast(Scene* out, Entity* entity, const Vec2& rayDirection, const char* mask, float maxDistance);