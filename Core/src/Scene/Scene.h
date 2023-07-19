#pragma once
#include "Entity.h"

struct Scene
{
	List Entities;

	uint32_t ViewportWidth;
	uint32_t ViewportHeight;
};

void Scene_Create(Scene& out);
void Scene_Ininialize(Scene& out);
void Scene_Destroy(Scene& out);
void Scene_AddEntity(Scene& out, const Entity& entity);
Entity* Scene_GetEntityByName(const Scene& out, const char* name);
Entity* Scene_GetPrimaryCamera(const Scene& out);
void Scene_OnUpdate(const Scene& out, float timeStep);
void Scene_OnViewportResize(Scene& out, uint32_t width, uint32_t height);