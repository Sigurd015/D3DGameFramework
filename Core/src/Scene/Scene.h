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

void Scene_Create(Scene& scene);
void Scene_Ininialize(Scene& scene);
void Scene_Destroy(Scene& scene);
void Scene_AddEntity(Scene& scene, Entity& entity);
Entity* Scene_GetEntityByName(const Scene* scene, const char* name);
Entity* Scene_GetPrimaryCamera(const Scene& scene);
void* Scene_GetListener(const Scene* scene);
void Scene_SetEntityEnabled(Entity* entity, bool enabled);
void Scene_OnUpdate(Scene& scene, float timeStep);
void Scene_OnViewportResize(Scene& scene, uint32_t width, uint32_t height);
bool Scene_Raycast(Scene* scene, Entity* entity, const Vec2& rayDirection, const char* mask, float maxDistance);